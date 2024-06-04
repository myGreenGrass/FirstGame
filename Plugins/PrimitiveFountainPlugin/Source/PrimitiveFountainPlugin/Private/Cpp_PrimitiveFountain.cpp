#include "Cpp_PrimitiveFountain.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"  // 引入FMath的头文件

// Sets default values
ACpp_PrimitiveFountain::ACpp_PrimitiveFountain()
{
    PrimaryActorTick.bCanEverTick = true;

    // 初始化喷泉设置的默认值
    MinSpawnInterval = 0.1f;
    MaxSpawnInterval = 1.0f;
    SpawnVelocity = 100.0f;
    SpawnArea = FVector(100.0f, 100.0f, 100.0f);
    MinScale = FVector(0.5f, 0.5f, 0.5f);
    MaxScale = FVector(2.0f, 2.0f, 2.0f);
    LaunchAngle = 45.0f;
    LifeTime = 5.0f;

    ParticlePoolSize = 100; // 设定粒子池大小
    CurrentIndex = 0; // 初始化当前粒子索引

    // 初始化粒子池
    ParticlePool.SetNum(ParticlePoolSize);
}

// Called when the game starts or when spawned
void ACpp_PrimitiveFountain::BeginPlay()
{
    Super::BeginPlay();

    // 设置下一次生成的时间
    TimeUntilNextSpawn = FMath::RandRange(MinSpawnInterval, MaxSpawnInterval);

    // 初始化每个粒子的属性
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

    // 更新粒子的状态
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

    // 检查是否需要生成新的粒子
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
