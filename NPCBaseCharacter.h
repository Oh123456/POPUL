// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseCharacter.h"
#include "NPCBaseCharacter.generated.h"

/**
 * 
 */
// NPC 타입
enum NPCType
{
	E_None  = 0,
	E_Shop	= 1,
};

UCLASS()
class POPUL_API ANPCBaseCharacter : public AEnemyBaseCharacter
{
	GENERATED_BODY()
public:
	bool GetNPCType() { return m_NPCType; };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// NPC의 타입
	int m_NPCType;
};
