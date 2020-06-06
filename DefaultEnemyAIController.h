// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "DefaultEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class POPUL_API ADefaultEnemyAIController : public AEnemyAIController
{
	GENERATED_BODY()
	
public:

	virtual int AttackBranch(int Rand) override;
};
