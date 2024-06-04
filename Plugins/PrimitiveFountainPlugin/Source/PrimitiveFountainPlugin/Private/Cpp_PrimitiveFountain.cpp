#include "Cpp_PrimitiveFountain.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"  // ����FMath��ͷ�ļ�

// Sets default values
ACpp_PrimitiveFountain::ACpp_PrimitiveFountain()
{
    PrimaryActorTick.bCanEverTick = true;

    // ��ʼ����Ȫ���õ�Ĭ��ֵ
    MinSpawnInterval = 0.1f;
    MaxSpawnInterval = 1.0f;
    SpawnVelocity = 100.0f;
    SpawnArea = FVector(100.0f, 100.0f, 100.0f);
    MinScale = FVector(0.5f, 0.5f, 0.5f);
    MaxScale = FVector(2.0f, 2.0f, 2.0f);
    LaunchAngle = 45.0f;
    LifeTime = 5.0f;

    ParticlePoolSize = 100; // �趨���ӳش�С
    CurrentIndex = 0; // ��ʼ����ǰ��������

    // ��ʼ�����ӳ�
    ParticlePool.SetNum(ParticlePoolSize);
}

// Called when the game starts or when spawned
void ACpp_PrimitiveFountain::BeginPlay()
{
    Super::BeginPlay();

    // ������һ�����ɵ�ʱ��
    TimeUntilNextSpawn = FMath::RandRange(MinSpawnInterval, MaxSpawnInterval);

    // ��ʼ��ÿ�����ӵ�����
    for (int i = 0; i < ParticlePoolSize; ++i)
    {
        FParticle& Particle = ParticlePool[i];

        Particle.MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
        Particle.MeshComponent->RegisterComponent();
        Particle.MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Particle.MeshComponent->SetVisibility(false);

        int MeshIndex = FMath::RandRange(0, MeshTypes.Num() - 1);
        if (MeshTypes.IsValidIndex(MeshIndex))
        {
            Particle.MeshComponent->SetStaticMesh(MeshTypes[MeshIndex]);
        }

        int MaterialIndex = FMath::RandRange(0, Materials.Num() - 1);
        if (Materials.IsValidIndex(MaterialIndex))
        {
            Particle.MeshComponent->SetMaterial(0, Materials[MaterialIndex]);
        }

        FVector Scale = FVector(
            FMath::RandRange(MinScale.X, MaxScale.X),
            FMath::RandRange(MinScale.Y, MaxScale.Y),
            FMath::RandRange(MinScale.Z, MaxScale.Z)
        );
        Particle.MeshComponent->SetWorldScale3D(Scale);
        Particle.LifeTime = LifeTime;
        Particle.bIsActive = false;
    }
}

// Called every frame
void ACpp_PrimitiveFountain::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // �������ӵ�״̬
    for (FParticle& Particle : ParticlePool)
    {
        if (Particle.bIsActive)
        {
            Particle.Location += Particle.InitialVelocity * DeltaTime;
            Particle.MeshComponent->SetWorldLocation(Particle.Location);

            Particle.LifeTime -= DeltaTime;
            if (Particle.LifeTime <= 0)
            {
                Particle.MeshComponent->SetVisibility(false);
                Particle.bIsActive = false;
            }
        }
    }

    // ����Ƿ���Ҫ�����µ�����
    TimeUntilNextSpawn -= DeltaTime;
    if (TimeUntilNextSpawn <= 0)
    {
        SpawnParticle();
        TimeUntilNextSpawn = FMath::RandRange(MinSpawnInterval, MaxSpawnInterval);
    }
}

void ACpp_PrimitiveFountain::SpawnParticle()
{
    if (ParticlePool.IsValidIndex(CurrentIndex))
    {
        FParticle& Particle = ParticlePool[CurrentIndex];

        Particle.InitialLocation = GetActorLocation() + FVector(
            FMath::RandRange(-SpawnArea.X / 2, SpawnArea.X / 2),
            FMath::RandRange(-SpawnArea.Y / 2, SpawnArea.Y / 2),
            FMath::RandRange(-SpawnArea.Z / 2, SpawnArea.Z / 2)
        );

        float AngleRad = FMath::DegreesToRadians(FMath::RandRange(-LaunchAngle, LaunchAngle));
        FVector Direction = FVector(FMath::Cos(AngleRad), 0, FMath::Sin(AngleRad));
        Particle.InitialVelocity = Direction * SpawnVelocity;

        Particle.Location = Particle.InitialLocation;
        Particle.LifeTime = LifeTime;
        Particle.bIsActive = true;

        Particle.MeshComponent->SetWorldLocation(Particle.InitialLocation);
        Particle.MeshComponent->SetVisibility(true);

        CurrentIndex = (CurrentIndex + 1) % ParticlePoolSize;
    }
}
