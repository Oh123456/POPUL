// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_FindPatrolPos.h"
#include "BTTask_RandomMove.generated.h"

/**
 * 
 */
UCLASS()
class POPUL_API UBTTask_RandomMove : public UBTTask_FindPatrolPos
{
	GENERATED_BODY()
	
public:
	UBTTask_RandomMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
