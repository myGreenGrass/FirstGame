// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"

//	初始化游戏模式

void AMyGameMode::InitGameState()
{
	Super::InitGameState();

	// 检查并设置默认角色类
	// 如果 DefaultPawnClass 等于 ADefaultPawn 类类型或 DefaultPawnClass 为 nullptr
	if (DefaultPawnClass == ADefaultPawn::StaticClass() || !DefaultPawnClass)
	{
		// 将 DefaultPawnClass 设置为 CustomXPCharacterClass
		DefaultPawnClass = CustomXPCharacterClass;
	}
}