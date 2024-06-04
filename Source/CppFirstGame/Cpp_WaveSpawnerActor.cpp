// ����Ŀ���õ�����ҳ����д��İ�Ȩ������

#include "Cpp_WaveSpawnerActor.h"  // ����WaveSpawnerActor���ͷ�ļ�
#include "Cpp_Enemy.h"  // ����Cpp_Enemy���ͷ�ļ�

// ���ô�Actor���Ե�Ĭ��ֵ
ACpp_WaveSpawnerActor::ACpp_WaveSpawnerActor()
{
	// ���ô�Actorÿ֡����Tick()���������Ҫ�����Թر���������ܡ�
	PrimaryActorTick.bCanEverTick = true;

	currentValue = 0;  // ��ʼ��currentValueΪ0
}

// ����Ϸ��ʼ�����ɴ�Actorʱ����
void ACpp_WaveSpawnerActor::BeginPlay()
{
	Super::BeginPlay();  // ���ø����BeginPlay()

	// ��Ӳ�����Waves�����У���ʾÿһ�����˵�����
	Waves.Add(6);
	Waves.Add(10);
	Waves.Add(20);

	SpawnWave();  // ���ɳ�ʼ��
}

// ÿ֡����һ��
void ACpp_WaveSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);  // ���ø����Tick()

}

// ����һ������
void ACpp_WaveSpawnerActor::SpawnWave()
{
	// ���currentValue���ڵ���3����ʾ�Ѿ����������в�����ʾ��WIN!���ı�
	if (currentValue >= 3)
	{
		MyText = FText::FromString(TEXT("WIN!"));
		return;
	}

	// ���WaveSound���ڣ��������ɲ�������
	if (WaveSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WaveSound, GetActorLocation());
	}

	// ������ʾ��ǰ�������ı�
	FText FormatPattern = FText::FromString(TEXT("Wave {0}"));
	MyText = FText::Format(FormatPattern, FText::AsNumber(currentValue + 1));

	// ���ݵ�ǰ�������ɵ���
	for (int i = 0; i < Waves[currentValue]; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, SpawnedWavePositions.Num() - 1);  // ���ѡ������λ�õ�����
		FTransform SpawnTransform = SpawnedWavePositions[RandomIndex];  // ��ȡ����λ��

		//FTransform SpawnTransform = GetActorTransform();  // Ҳ����ʹ��Actor�����λ��



		// // ȷ������λ��û����ײ
		//FVector SpawnLocation = SpawnTransform.GetLocation();
		//if (GetWorld()->OverlapBlockingTestByChannel(SpawnLocation, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(50.0f)))
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Spawn location blocked at [%s], trying to adjust"), *SpawnLocation.ToString());
		//	SpawnLocation.Z += 100.0f;  // ����λ���Ա�����ײ
		//	SpawnTransform.SetLocation(SpawnLocation);
		//}
		//// ��ָ��λ�����ɵ���
		//FActorSpawnParameters SpawnParams;
		//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;




		// ��ָ��λ�����ɵ���
		if (AActor* SpawnedActor = GetWorld()->SpawnActor(Enemy, &SpawnTransform))
		{
			// ת��ΪCpp_Enemy����
			ACpp_Enemy* Tmp_Enemy = Cast<ACpp_Enemy>(SpawnedActor);
			// ����Ĭ�Ͽ�����
			Tmp_Enemy->SpawnDefaultController();
			UE_LOG(LogTemp, Log, TEXT("Successfully spawned enemy"));  // ��ӡ�ɹ����ɵ��˵���־
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn enemy"));  // ��ӡ���ɵ���ʧ�ܵľ�����־
		}
	}
	currentValue++;  // ����currentValue��������һ��
}

// �������Ƿ����
void ACpp_WaveSpawnerActor::CheckForEnemy()
{
	if (GetWorld())
	{
		// ���ö�ʱ���Լ����˲�������һ��
		GetWorld()->GetTimerManager().SetTimer(CheckEnemiesTimerHandle, this, &ACpp_WaveSpawnerActor::HandleNextWave, 0.2f, false);
	}
}

// ������һ������
void ACpp_WaveSpawnerActor::HandleNextWave()
{
	TArray<AActor*> FoundEnemies;  // �洢�ҵ��ĵ���
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACpp_Enemy::StaticClass(), FoundEnemies);  // ��ȡ����Cpp_Enemy���͵�Actor

	// ���û���ҵ����ˣ������µ�һ��
	if (FoundEnemies.Num() == 0)
	{
		// û�е��ˣ������µĵ��˲�
		SpawnWave();
	}
}
