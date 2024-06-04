// 在项目设置的描述页面填写你的版权声明。

#include "Cpp_WaveSpawnerActor.h"  // 包含WaveSpawnerActor类的头文件
#include "Cpp_Enemy.h"  // 包含Cpp_Enemy类的头文件

// 设置此Actor属性的默认值
ACpp_WaveSpawnerActor::ACpp_WaveSpawnerActor()
{
	// 设置此Actor每帧调用Tick()。如果不需要，可以关闭以提高性能。
	PrimaryActorTick.bCanEverTick = true;

	currentValue = 0;  // 初始化currentValue为0
}

// 在游戏开始或生成此Actor时调用
void ACpp_WaveSpawnerActor::BeginPlay()
{
	Super::BeginPlay();  // 调用父类的BeginPlay()

	// 添加波数到Waves数组中，表示每一波敌人的数量
	Waves.Add(6);
	Waves.Add(10);
	Waves.Add(20);

	SpawnWave();  // 生成初始波
}

// 每帧调用一次
void ACpp_WaveSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);  // 调用父类的Tick()

}

// 生成一波敌人
void ACpp_WaveSpawnerActor::SpawnWave()
{
	// 如果currentValue大于等于3，表示已经生成完所有波，显示“WIN!”文本
	if (currentValue >= 3)
	{
		MyText = FText::FromString(TEXT("WIN!"));
		return;
	}

	// 如果WaveSound存在，播放生成波的声音
	if (WaveSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WaveSound, GetActorLocation());
	}

	// 设置显示当前波数的文本
	FText FormatPattern = FText::FromString(TEXT("Wave {0}"));
	MyText = FText::Format(FormatPattern, FText::AsNumber(currentValue + 1));

	// 根据当前波数生成敌人
	for (int i = 0; i < Waves[currentValue]; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, SpawnedWavePositions.Num() - 1);  // 随机选择生成位置的索引
		FTransform SpawnTransform = SpawnedWavePositions[RandomIndex];  // 获取生成位置

		//FTransform SpawnTransform = GetActorTransform();  // 也可以使用Actor自身的位置



		// // 确保生成位置没有碰撞
		//FVector SpawnLocation = SpawnTransform.GetLocation();
		//if (GetWorld()->OverlapBlockingTestByChannel(SpawnLocation, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(50.0f)))
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Spawn location blocked at [%s], trying to adjust"), *SpawnLocation.ToString());
		//	SpawnLocation.Z += 100.0f;  // 调整位置以避免碰撞
		//	SpawnTransform.SetLocation(SpawnLocation);
		//}
		//// 在指定位置生成敌人
		//FActorSpawnParameters SpawnParams;
		//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;




		// 在指定位置生成敌人
		if (AActor* SpawnedActor = GetWorld()->SpawnActor(Enemy, &SpawnTransform))
		{
			// 转换为Cpp_Enemy类型
			ACpp_Enemy* Tmp_Enemy = Cast<ACpp_Enemy>(SpawnedActor);
			// 生成默认控制器
			Tmp_Enemy->SpawnDefaultController();
			UE_LOG(LogTemp, Log, TEXT("Successfully spawned enemy"));  // 打印成功生成敌人的日志
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn enemy"));  // 打印生成敌人失败的警告日志
		}
	}
	currentValue++;  // 增加currentValue以生成下一波
}

// 检查敌人是否存在
void ACpp_WaveSpawnerActor::CheckForEnemy()
{
	if (GetWorld())
	{
		// 设置定时器以检查敌人并处理下一波
		GetWorld()->GetTimerManager().SetTimer(CheckEnemiesTimerHandle, this, &ACpp_WaveSpawnerActor::HandleNextWave, 0.2f, false);
	}
}

// 处理下一波生成
void ACpp_WaveSpawnerActor::HandleNextWave()
{
	TArray<AActor*> FoundEnemies;  // 存储找到的敌人
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACpp_Enemy::StaticClass(), FoundEnemies);  // 获取所有Cpp_Enemy类型的Actor

	// 如果没有找到敌人，生成新的一波
	if (FoundEnemies.Num() == 0)
	{
		// 没有敌人，生成新的敌人波
		SpawnWave();
	}
}
