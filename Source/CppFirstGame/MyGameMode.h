// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cpp_Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/DefaultPawn.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CPPFIRSTGAME_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

	void InitGameState() override;

//public:
	// �û���ɫ��
	UPROPERTY(EditAnywhere, NoClear)
	TSubclassOf<ACpp_Character> CustomXPCharacterClass = ACpp_Character::StaticClass();
	
};
