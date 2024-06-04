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
	// 蓝图可调用函数，用于生成一波敌人
	UFUNCTION(BlueprintCallable)
	void SpawnWave();

	// 当前波次值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 currentValue;

	// 保存所有波次的数组
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<int32> Waves;

	// 保存每波生成位置的数组
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TArray<FTransform> SpawnedWavePositions;


private:

	// 波次生成时播放的声音
	UPROPERTY(EditAnywhere, Category = "Spawn")
	USoundBase* WaveSound;

	// 用于生成敌人的组件数组
	UPROPERTY(VisibleAnywhere, Category = "Spawn")
	TArray<USceneComponent*> SpawnComponents;

public:
	// 敌人类的子类，蓝图可读写
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> Enemy;

	// 检查敌人是否还存在的函数
	UFUNCTION()
	void CheckForEnemy();

private:
	// 检查敌人的计时器句柄
	FTimerHandle CheckEnemiesTimerHandle;

public:
	// 处理下一波敌人的函数
	UFUNCTION()
	void HandleNextWave();

protected:
	// UI显示的文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText MyText;
};
