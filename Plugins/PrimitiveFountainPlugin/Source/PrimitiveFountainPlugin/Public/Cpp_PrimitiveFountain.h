#pragma once  // 防止头文件被多次包含

#include "CoreMinimal.h"  // 引入核心最小化的头文件
#include "GameFramework/Actor.h"  // 引入Actor类的头文件
#include "Components/StaticMeshComponent.h"  // 引入StaticMeshComponent的头文件
#include "Materials/Material.h"  // 引入Material的头文件
#include "Engine/StaticMesh.h"  // 引入StaticMesh的头文件
#include "UObject/ConstructorHelpers.h"  // 引入ConstructorHelpers的头文件
#include "Engine/World.h"  // 引入World相关的头文件
#include "Materials/MaterialInstanceDynamic.h"  // 引入MaterialInstanceDynamic的头文件
#include "Kismet/GameplayStatics.h"  // 引入GameplayStatics的头文件

#include "Cpp_PrimitiveFountain.generated.h"

// 定义一个粒子结构体，用于存储粒子的相关属性
USTRUCT(BlueprintType)
struct FParticle
{
    GENERATED_BODY()

    UStaticMeshComponent* MeshComponent;  // 粒子的静态网格组件

    FVector Size;  // 粒子的大小

    FVector InitialVelocity;  // 粒子的初始速度

    FVector InitialLocation;  // 粒子的初始位置

    FVector Location;  // 粒子的当前位置

    float LifeTime;  // 粒子的生命周期

    bool bIsActive;  // 粒子是否处于活跃状态
};

UCLASS()
class PRIMITIVEFOUNTAINPLUGIN_API ACpp_PrimitiveFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACpp_PrimitiveFountain();

protected:
    // 在游戏开始或生成此Actor时调用
    virtual void BeginPlay() override;

public:	
    // 每帧调用一次
	virtual void Tick(float DeltaTime) override;

public:
    // 喷泉设置，编辑器可编辑，蓝图可读写，存储静态网格类型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    TArray<UStaticMesh*> MeshTypes;

    // 喷泉设置，编辑器可编辑，蓝图可读写，存储材质类型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    TArray<UMaterial*> Materials;

    // 喷泉设置，编辑器可编辑，蓝图可读写，存储最小生成间隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float MinSpawnInterval;

    // 喷泉设置，编辑器可编辑，蓝图可读写，存储最大生成间隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float MaxSpawnInterval;

    // 喷泉设置，编辑器可编辑，蓝图可读写，存储生成速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float SpawnVelocity;

    // 喷泉设置，编辑器可编辑，蓝图可读写，存储生成区域
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector SpawnArea;

    // 喷泉设置，编辑器可编辑，蓝图可读写，存储最小缩放比例
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector MinScale;

    // 喷泉设置，编辑器可编辑，蓝图可读写，存储最大缩放比例
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector MaxScale;

    // 喷泉设置，编辑器可编辑，蓝图可读写，存储发射角度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float LaunchAngle;

    // 喷泉设置，编辑器可编辑，蓝图可读写，存储粒子生命周期
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float LifeTime;

private:
    // 生成粒子函数
    void SpawnParticle();

    // 距离下一次生成的时间
    float TimeUntilNextSpawn;

    // 粒子池数组，用于管理粒子
    TArray<FParticle> ParticlePool;
    int ParticlePoolSize;  // 粒子池大小

    // 当前粒子的索引
    int CurrentIndex;

};
