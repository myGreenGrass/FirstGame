#pragma once  // ��ֹͷ�ļ�����ΰ���

#include "CoreMinimal.h"  // ���������С����ͷ�ļ�
#include "GameFramework/Actor.h"  // ����Actor���ͷ�ļ�
#include "Components/StaticMeshComponent.h"  // ����StaticMeshComponent��ͷ�ļ�
#include "Materials/Material.h"  // ����Material��ͷ�ļ�
#include "Engine/StaticMesh.h"  // ����StaticMesh��ͷ�ļ�
#include "UObject/ConstructorHelpers.h"  // ����ConstructorHelpers��ͷ�ļ�
#include "Engine/World.h"  // ����World��ص�ͷ�ļ�
#include "Materials/MaterialInstanceDynamic.h"  // ����MaterialInstanceDynamic��ͷ�ļ�
#include "Kismet/GameplayStatics.h"  // ����GameplayStatics��ͷ�ļ�

#include "Cpp_PrimitiveFountain.generated.h"

// ����һ�����ӽṹ�壬���ڴ洢���ӵ��������
USTRUCT(BlueprintType)
struct FParticle
{
    GENERATED_BODY()

    UStaticMeshComponent* MeshComponent;  // ���ӵľ�̬�������

    FVector Size;  // ���ӵĴ�С

    FVector InitialVelocity;  // ���ӵĳ�ʼ�ٶ�

    FVector InitialLocation;  // ���ӵĳ�ʼλ��

    FVector Location;  // ���ӵĵ�ǰλ��

    float LifeTime;  // ���ӵ���������

    bool bIsActive;  // �����Ƿ��ڻ�Ծ״̬
};

UCLASS()
class PRIMITIVEFOUNTAINPLUGIN_API ACpp_PrimitiveFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACpp_PrimitiveFountain();

protected:
    // ����Ϸ��ʼ�����ɴ�Actorʱ����
    virtual void BeginPlay() override;

public:	
    // ÿ֡����һ��
	virtual void Tick(float DeltaTime) override;

public:
    // ��Ȫ���ã��༭���ɱ༭����ͼ�ɶ�д���洢��̬��������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    TArray<UStaticMesh*> MeshTypes;

    // ��Ȫ���ã��༭���ɱ༭����ͼ�ɶ�д���洢��������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    TArray<UMaterial*> Materials;

    // ��Ȫ���ã��༭���ɱ༭����ͼ�ɶ�д���洢��С���ɼ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float MinSpawnInterval;

    // ��Ȫ���ã��༭���ɱ༭����ͼ�ɶ�д���洢������ɼ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float MaxSpawnInterval;

    // ��Ȫ���ã��༭���ɱ༭����ͼ�ɶ�д���洢�����ٶ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float SpawnVelocity;

    // ��Ȫ���ã��༭���ɱ༭����ͼ�ɶ�д���洢��������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector SpawnArea;

    // ��Ȫ���ã��༭���ɱ༭����ͼ�ɶ�д���洢��С���ű���
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector MinScale;

    // ��Ȫ���ã��༭���ɱ༭����ͼ�ɶ�д���洢������ű���
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector MaxScale;

    // ��Ȫ���ã��༭���ɱ༭����ͼ�ɶ�д���洢����Ƕ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float LaunchAngle;

    // ��Ȫ���ã��༭���ɱ༭����ͼ�ɶ�д���洢������������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float LifeTime;

private:
    // �������Ӻ���
    void SpawnParticle();

    // ������һ�����ɵ�ʱ��
    float TimeUntilNextSpawn;

    // ���ӳ����飬���ڹ�������
    TArray<FParticle> ParticlePool;
    int ParticlePoolSize;  // ���ӳش�С

    // ��ǰ���ӵ�����
    int CurrentIndex;

};
