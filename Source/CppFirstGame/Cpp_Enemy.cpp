#include "Cpp_Enemy.h"

// ����Ĭ������ֵ
ACpp_Enemy::ACpp_Enemy()
{
    // ���øý�ɫÿ֡���� Tick() ����
    PrimaryActorTick.bCanEverTick = true;

    // ��������׷��λ������������丽�ӵ��������
    AttackTracePos = CreateDefaultSubobject<UArrowComponent>(TEXT("Attack Trace Pos"));
    AttackTracePos->SetupAttachment(GetCapsuleComponent());

    // ��ʼ�������˺��������뾶���Ƿ��ܹ�����־������ֵ
    BaseDamage = 10.0f;
    AttackRadius = 25.0f;
    CanAttack = true;
    Health = 10.0f;  // ��ʼ������ֵΪ10

}

// 0.��Ϸ��ʼ������ʱ����
void ACpp_Enemy::BeginPlay()
{
    Super::BeginPlay();

    // ��ȡ����ʵ��
    AnimInstance = GetMesh()->GetAnimInstance();


    // ��Ӷ���֪ͨ��ʼʱ�Ļص�����
    AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ACpp_Enemy::OnNotifyBegin);
    // ��Ӷ�����̫�����ʱ�Ļص�����
    AnimInstance->OnMontageEnded.AddDynamic(this, &ACpp_Enemy::OnAttackMontageEnded);

    // ����һ����ʱ������ȡAI������
    GetWorldTimerManager().SetTimer(TimerHandle_GetAIController, this, &ACpp_Enemy::GetAIController, 0.5f, false);

    // �����������
    SetRandomMesh();

}

// ÿ֡����һ��
void ACpp_Enemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// �����빦��
void ACpp_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 1.1׷�����
void ACpp_Enemy::ChasePlayer()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    APawn* PlayerPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

    if (PlayerPawn)
    {
        if (AIController)
        {
            // ͨ��AI��Ϊ���Զ�׷�����
            AIController->MoveToActor(PlayerPawn, AcceptanceRadius, true, true, false, 0, true);
        }
    }
}

// 1.3 AI�ƶ����ʱ���õĺ���
void ACpp_Enemy::OnAIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    if (Result == EPathFollowingResult::Success)
    {
        if (CanAttack && PushMontage)
        {
            CanAttack = false;

            // ���Ź���������̫��
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(PushMontage);
            }
        }
    }
}

// 1.2��ȡAI������
void ACpp_Enemy::GetAIController()
{
    AIController = Cast<AAIController>(GetController());

    if (AIController)
    {
        // ����ƶ����ʱ�Ļص�����
        AIController->ReceiveMoveCompleted.AddDynamic(this, &ACpp_Enemy::OnAIMoveCompleted);
        ChasePlayer();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AIController is null"));
        GetWorldTimerManager().SetTimer(TimerHandle_GetAIController, this, &ACpp_Enemy::GetAIController, 0.5f, false);
    }
}

// 0.2  �����������
void ACpp_Enemy::SetRandomMesh()
{
    if (Meshes.Num() > 0)
    {
        int RandomNumber = FMath::RandRange(0, Meshes.Num() - 1);
        USkeletalMesh* RandomMesh = Meshes[RandomNumber];

        // ��������
        GetMesh()->SetSkeletalMesh(RandomMesh);

    }


}

// 2. ֪ͨ��ʼʱ���õĻص�����
void ACpp_Enemy::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    // ��ȡ��ʼλ�úͷ���
    FVector Start = AttackTracePos->GetComponentLocation();
    FVector ForwardVector = AttackTracePos->GetForwardVector();
    FVector End = Start + (ForwardVector * 100.0f);

    // ���ú��Ե���Ա����
    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(this);

    // ���ö�����������
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    // ��������׷��
    FHitResult HitResult;
    bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
        this,
        Start,
        End,
        AttackRadius,
        ObjectTypes,
        false,
        IgnoredActors,
        EDrawDebugTrace::None,
        HitResult,
        true
    );

    if (bHit)
    {
        // ���������߼�
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UGameplayStatics::ApplyDamage(HitActor, BaseDamage, GetController(), this, nullptr);
        }
    }
}

// 3. ������̫�����ʱ���õĻص�����
void ACpp_Enemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == PushMontage)
    {
        CanAttack = true;

        // ����׷�����
        ChasePlayer();
    }
}