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

// ����һ���ಥί�У������Զ����¼�
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChasePlayerDelegate);

// ���� Cpp_Enemy �࣬�̳��� ACharacter
UCLASS()
class CPPFIRSTGAME_API ACpp_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// ���캯��������Ĭ������ֵ
	ACpp_Enemy();

protected:
	// ��Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;

public:	
	// ÿ֡����һ��
	virtual void Tick(float DeltaTime) override;

	// �����빦��
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// AI�ƶ����ʱ���õĺ���
	UFUNCTION()
	void OnAIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	// ��������˺�
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// �����˲��ݻ�
	void CheckForEnemiesAndDestroy();

private:

	// ������̫��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* PushMontage;

	// ����׷��λ��
	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* AttackTracePos;

	// ���ܰ뾶
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius = 5.0;

	// �˺�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	USoundBase* DamageSound;

	// �����˺�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	// �����뾶
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	// ����ֵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;

	// �ܷ񹥻���־
	bool CanAttack;

	// AI������
	AAIController* AIController;

	// �ӳټ�ʱ�����
	FTimerHandle DelayTimerHandle;

	// ����ʵ��
	UAnimInstance* AnimInstance;

	// ��ȡAI�������ļ�ʱ�����
	FTimerHandle TimerHandle_GetAIController;
	 //֪ͨ��ʼʱ���õĻص�����
	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	// ������̫�����ʱ���õĻص�����
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:

	// �ƶ��ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

	// ת���ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnSpeed = 100.f;

	// ������������
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TArray<USkeletalMesh*> Meshes;

private:
	// ׷�����
	void ChasePlayer();

	// �����������
	void SetRandomMesh();

	// ��ȡAI������
	void GetAIController();
};
