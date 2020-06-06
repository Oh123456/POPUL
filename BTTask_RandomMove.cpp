// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_RandomMove.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyAIController.h"

UBTTask_RandomMove::UBTTask_RandomMove()
{
	NodeName = TEXT("RandomMove");
}

EBTNodeResult::Type UBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllPawn == nullptr)
		return EBTNodeResult::Type::Failed;
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::HomePosKey, ControllPawn->GetActorLocation());

	return EBTNodeResult::Type::Succeeded;
}