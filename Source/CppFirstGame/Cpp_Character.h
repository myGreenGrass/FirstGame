#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Blueprint/UserWidget.h"

#include "Cpp_Character.generated.h"

// 声明角色类
UCLASS()
class CPPFIRSTGAME_API ACpp_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// 设置该角色的默认属性
	ACpp_Character();

protected:
	// 在游戏开始或生成时调用
	virtual void BeginPlay() override;

	// 每帧调用一次
	void Tick(float DeltaTime);

	// 设置玩家输入组件
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	// 处理角色受到的伤害
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// 公有成员

private:
	// 私有成员

	// 动画蒙太奇，用于推的动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* PushMontage;

	// 玩家相机组件
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* PlayerCamera;

	// 攻击轨迹位置的箭头组件
	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* AttackTracePos;

	// 弹簧臂组件，用于控制相机的距离
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComponent;

	// 受伤音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	USoundBase* DamageSound;

	// 基础伤害值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	// 攻击半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	// 当前生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;

	// 最大生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	// 能否攻击的标志
	bool bCanAttack;

	// 动画蒙太奇结束时的回调函数
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 动画通知开始时的回调函数
	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

protected:
	// 增强输入相关属性

	// 输入映射上下文
	/*UPROPERTY(EditAnywhere, Category = "EnhancedInput")*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapping;

	// 移动动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	// 视角移动动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	// 攻击动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	// 移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

	// 转向速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnSpeed = 100.f * 6;

private:
	// 处理移动输入
	void Movement(const FInputActionValue& ActionValue);

	// 处理视角移动输入
	void Look(const FInputActionValue& ActionValue);

	// 处理攻击输入
	void Attack();

	// 动画实例
	UAnimInstance* AnimInstance;

public:
	// 创建生命条组件
	void CreateHealthBarWidget();

	// 生命条UI的类
	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<UUserWidget> HealthBarWidgetClass;

	// 攻击HUD的类
	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<UUserWidget> AttackHUDWidgetClass;

protected:
	// 生命条UI
	//UUserWidget* HealthBarWidget;

	// 攻击HUD
	//UPROPERTY(BlueprintReadWrite)
	//UUserWidget* AttackHUDWidget;
};