
// 填写版权声明信息在项目设置页面的描述中
#include "Cpp_Character.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

// 设置默认值
ACpp_Character::ACpp_Character()
{
	// 创建弹簧臂组件并设置属性
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->TargetArmLength = 300.0f; // 相机跟随在角色背后的距离
	SpringArmComponent->bUsePawnControlRotation = true; // 根据控制器旋转臂

	// 创建玩家相机组件并设置属性
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArmComponent);
	PlayerCamera->bUsePawnControlRotation = false; // 相机不相对于臂旋转

	// 创建攻击轨迹位置的箭头组件
	AttackTracePos = CreateDefaultSubobject<UArrowComponent>(TEXT("Attack Trace Pos"));
	AttackTracePos->SetupAttachment(GetCapsuleComponent());

	// 参数设置
	BaseDamage = 10.0f;		// 基础攻击伤害
	AttackRadius = 25.0f;	// 攻击范围
	bCanAttack = true;		// 能否攻击
	MaxHealth = 200.f;		// 最大生命
	Health = 200.0f; // 设置初始生命值为100
}

// 开始游戏时或角色生成时调用
void ACpp_Character::BeginPlay()
{

	UE_LOG(LogTemp, Warning, TEXT("ACpp_Character::BeginPlay start"));

	Super::BeginPlay();

	// 获取动画实例
	AnimInstance = GetMesh()->GetAnimInstance();

	// 绑定动画通知开始时的回调函数
	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ACpp_Character::OnNotifyBegin);

	// 绑定攻击动画结束时的回调函数
	AnimInstance->OnMontageEnded.AddDynamic(this, &ACpp_Character::OnAttackMontageEnded);

	// 创建生命条UI
	CreateHealthBarWidget();
}

void ACpp_Character::Tick(float DeltaTime)
{
	// 设置每帧调用 Tick() 函数，如果不需要可以关闭以提高性能
	PrimaryActorTick.bCanEverTick = true;

}

// 移动输入处理函数
void ACpp_Character::Movement(const FInputActionValue& ActionValue)
{
	FVector2D InputVector = ActionValue.Get<FVector2D>();

	// 获取控制器的旋转
	FRotator ControlRotation = Controller != nullptr ? Controller->GetControlRotation() : FRotator::ZeroRotator;
	// 使用Yaw旋转平面移动
	FRotator YawRotation(0, ControlRotation.Yaw, 0);

	// 计算前进和右移的向量
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 计算最终移动向量
	FVector MoveVector = ForwardVector * InputVector.Y + RightVector * InputVector.X;

	// 添加移动输入
	AddMovementInput(MoveVector, MoveSpeed * GetWorld()->GetDeltaSeconds());
}

// 视角移动输入处理函数
void ACpp_Character::Look(const FInputActionValue& ActionValue)
{
	FVector2D LookInput = ActionValue.Get<FVector2D>();
	AddControllerYawInput(LookInput.X * TurnSpeed * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(-1.f * LookInput.Y * TurnSpeed * GetWorld()->GetDeltaSeconds());
}

// 攻击输入处理函数
void ACpp_Character::Attack()
{
	if (bCanAttack && PushMontage)
	{
		bCanAttack = false;

		// 播放攻击动画
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(PushMontage);
		}
	}
}

// 创建生命条UI组件
void ACpp_Character::CreateHealthBarWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("创建UI画面"));

	if (HealthBarWidgetClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			APlayerController* PlayerController = World->GetFirstPlayerController();
			if (PlayerController)
			{
				// 创建生命条UI组件并添加到视口
				HealthBarWidget = CreateWidget<UUserWidget>(World, HealthBarWidgetClass);
				if (HealthBarWidget)
				{
					HealthBarWidget->AddToViewport();
				}

				// 创建攻击HUD组件并添加到视口
				AttackHUDWidget = CreateWidget<UUserWidget>(World, AttackHUDWidgetClass);
				if (AttackHUDWidget)
				{
					UE_LOG(LogTemp, Warning, TEXT("创建 AttackHUDWidget"));
					AttackHUDWidget->AddToViewport();
				}
			}
		}
	}
}

// 绑定输入功能
void ACpp_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 添加增强输入映射上下文
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			if (InputMapping)
			{
				//Subsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
				Subsystem->AddMappingContext(InputMapping, 0);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("InputMapping is null!"));
			}
		}
	}

	// 绑定输入动作
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACpp_Character::Movement);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACpp_Character::Look);
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACpp_Character::Attack);
	}
}

// 攻击动画结束时的回调函数
void ACpp_Character::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == PushMontage)
	{
		bCanAttack = true;
	}
}

void ACpp_Character::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UE_LOG(LogTemp, Warning, TEXT("开始攻击！"));
	// 获取攻击位置和方向
	FVector Start = AttackTracePos->GetComponentLocation();
	FVector ForwardVector = AttackTracePos->GetForwardVector();
	FVector End = Start + (ForwardVector * 100.0f);

	// 设置攻击判定参数
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	FHitResult HitResult;
	// 进行球形射线检测以判断是否有角色受到攻击
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

	// 如果检测到了碰撞
	if (bHit)
	{
		// 处理攻击命中逻辑
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UGameplayStatics::ApplyDamage(HitActor, BaseDamage, GetController(), this, nullptr);
		}
	}
}

// 受到伤害时的处理函数
float ACpp_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 调用父类的受伤处理函数
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 减少角色生命值
	Health -= DamageAmount;

	// 播放受伤声音
	UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetActorLocation());

	// 如果生命值小于等于0，处理角色死亡逻辑
	if (Health <= 0)
	{
		Health = 0;

		// 禁用输入
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			DisableInput(PlayerController);
		}

		// 启用模拟物理
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(TEXT("Custom")); // 设置碰撞预设为自定义
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 启用查询和物理碰撞
		GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); // 设置所有碰撞通道的默认响应
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

		bCanAttack = false;
	}
	else
	{
		// 执行其他受伤逻辑
		FVector LaunchVelocity = GetActorForwardVector() * -200.0f + GetActorUpVector() * 100.0f;
		LaunchCharacter(LaunchVelocity, true, true);
	}

	return DamageAmount;
}
