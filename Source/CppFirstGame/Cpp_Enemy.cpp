#include "Cpp_Enemy.h"

// 设置默认属性值
ACpp_Enemy::ACpp_Enemy()
{
    // 设置该角色每帧调用 Tick() 函数
    PrimaryActorTick.bCanEverTick = true;

    // 创建攻击追踪位置组件，并将其附加到胶囊组件
    AttackTracePos = CreateDefaultSubobject<UArrowComponent>(TEXT("Attack Trace Pos"));
    AttackTracePos->SetupAttachment(GetCapsuleComponent());

    // 初始化基础伤害、攻击半径、是否能攻击标志和生命值
    BaseDamage = 10.0f;
    AttackRadius = 25.0f;
    CanAttack = true;
    Health = 10.0f;  // 初始化生命值为10

}

// 0.游戏开始或生成时调用
void ACpp_Enemy::BeginPlay()
{
    Super::BeginPlay();

    // 获取动画实例
    AnimInstance = GetMesh()->GetAnimInstance();


    // 添加动画通知开始时的回调函数
    AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ACpp_Enemy::OnNotifyBegin);
    // 添加动画蒙太奇结束时的回调函数
    AnimInstance->OnMontageEnded.AddDynamic(this, &ACpp_Enemy::OnAttackMontageEnded);

    // 设置一个计时器来获取AI控制器
    GetWorldTimerManager().SetTimer(TimerHandle_GetAIController, this, &ACpp_Enemy::GetAIController, 0.5f, false);

    // 设置随机网格
    SetRandomMesh();

}

// 每帧调用一次
void ACpp_Enemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// 绑定输入功能
void ACpp_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 1.1追逐玩家
void ACpp_Enemy::ChasePlayer()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    APawn* PlayerPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

    if (PlayerPawn)
    {
        if (AIController)
        {
            // 通过AI行为树自动追踪玩家
            AIController->MoveToActor(PlayerPawn, AcceptanceRadius, true, true, false, 0, true);
        }
    }
}

// 1.3 AI移动完成时调用的函数
void ACpp_Enemy::OnAIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    if (Result == EPathFollowingResult::Success)
    {
        if (CanAttack && PushMontage)
        {
            CanAttack = false;

            // 播放攻击动画蒙太奇
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(PushMontage);
            }
        }
    }
}

// 1.2获取AI控制器
void ACpp_Enemy::GetAIController()
{
    AIController = Cast<AAIController>(GetController());

    if (AIController)
    {
        // 添加移动完成时的回调函数
        AIController->ReceiveMoveCompleted.AddDynamic(this, &ACpp_Enemy::OnAIMoveCompleted);
        ChasePlayer();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AIController is null"));
        GetWorldTimerManager().SetTimer(TimerHandle_GetAIController, this, &ACpp_Enemy::GetAIController, 0.5f, false);
    }
}

// 0.2  随机骨骼网络
void ACpp_Enemy::SetRandomMesh()
{
    if (Meshes.Num() > 0)
    {
        int RandomNumber = FMath::RandRange(0, Meshes.Num() - 1);
        USkeletalMesh* RandomMesh = Meshes[RandomNumber];

        // 设置网格
        GetMesh()->SetSkeletalMesh(RandomMesh);

    }


}

// 2. 通知开始时调用的回调函数
void ACpp_Enemy::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    // 获取起始位置和方向
    FVector Start = AttackTracePos->GetComponentLocation();
    FVector ForwardVector = AttackTracePos->GetForwardVector();
    FVector End = Start + (ForwardVector * 100.0f);

    // 配置忽略的演员数组
    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(this);

    // 配置对象类型数组
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    // 进行球体追踪
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
        // 处理命中逻辑
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UGameplayStatics::ApplyDamage(HitActor, BaseDamage, GetController(), this, nullptr);
        }
    }
}

// 3. 动画蒙太奇结束时调用的回调函数
void ACpp_Enemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == PushMontage)
    {
        CanAttack = true;

        // 继续追逐玩家
        ChasePlayer();
    }
}