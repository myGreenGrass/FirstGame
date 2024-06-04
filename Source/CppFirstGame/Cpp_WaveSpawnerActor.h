// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_WaveSpawnerActor.generated.h"

UCLASS()
class CPPFIRSTGAME_API ACpp_WaveSpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACpp_WaveSpawnerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ��ͼ�ɵ��ú�������������һ������
	UFUNCTION(BlueprintCallable)
	void SpawnWave();

	// ��ǰ����ֵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 currentValue;

	// �������в��ε�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<int32> Waves;

	// ����ÿ������λ�õ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TArray<FTransform> SpawnedWavePositions;


private:

	// ��������ʱ���ŵ�����
	UPROPERTY(EditAnywhere, Category = "Spawn")
	USoundBase* WaveSound;

	// �������ɵ��˵��������
	UPROPERTY(VisibleAnywhere, Category = "Spawn")
	TArray<USceneComponent*> SpawnComponents;

public:
	// ����������࣬��ͼ�ɶ�д
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> Enemy;

	// �������Ƿ񻹴��ڵĺ���
	UFUNCTION()
	void CheckForEnemy();

private:
	// �����˵ļ�ʱ�����
	FTimerHandle CheckEnemiesTimerHandle;

public:
	// ������һ�����˵ĺ���
	UFUNCTION()
	void HandleNextWave();

protected:
	// UI��ʾ���ı�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText MyText;
};
