// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"

//	��ʼ����Ϸģʽ

void AMyGameMode::InitGameState()
{
	Super::InitGameState();

	// ��鲢����Ĭ�Ͻ�ɫ��
	// ��� DefaultPawnClass ���� ADefaultPawn �����ͻ� DefaultPawnClass Ϊ nullptr
	if (DefaultPawnClass == ADefaultPawn::StaticClass() || !DefaultPawnClass)
	{
		// �� DefaultPawnClass ����Ϊ CustomXPCharacterClass
		DefaultPawnClass = CustomXPCharacterClass;
	}
}