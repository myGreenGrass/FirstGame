// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include <Runtime/AIModule/Classes/AITypes.h>
#include "TimerManager.h"

#include "Cpp_Enemy.generated.h"

// 声明一个多播委托，用于自定义事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChasePlayerDelegate);

// 声明 Cpp_Enemy 类，继承自 ACharacter
UCLASS()
class CPPFIRSTGAME_API ACpp_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// 构造函数，设置默认属性值
	ACpp_Enemy();

protected:
	// 游戏开始或生成时调用
	virtual void BeginPlay() override;

public:	
	// 每帧调用一次
	virtual void Tick(float DeltaTime) override;

	// 绑定输入功能
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// AI移动完成时调用的函数
	UFUNCTION()
	void OnAIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	// 接受玩家伤害
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 检查敌人并摧毁
	void CheckForEnemiesAndDestroy();

private:

	// 动画蒙太奇
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* PushMontage;

	// 攻击追踪位置
	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* AttackTracePos;

	// 接受半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius = 5.0;

	// 伤害声音
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	USoundBase* DamageSound;

	// 基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	// 攻击半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	// 生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;

	// 能否攻击标志
	bool CanAttack;

	// AI控制器
	AAIController* AIController;

	// 延迟计时器句柄
	FTimerHandle DelayTimerHandle;

	// 动画实例
	UAnimInstance* AnimInstance;

	// 获取AI控制器的计时器句柄
	FTimerHandle TimerHandle_GetAIController;
	 //通知开始时调用的回调函数
	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	// 动画蒙太奇结束时调用的回调函数
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:

	// 移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

	// 转向速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnSpeed = 100.f;

	// 骨骼网格数组
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TArray<USkeletalMesh*> Meshes;

private:
	// 追逐玩家
	void ChasePlayer();

	// 设置随机网格
	void SetRandomMesh();

	// 获取AI控制器
	void GetAIController();
};
