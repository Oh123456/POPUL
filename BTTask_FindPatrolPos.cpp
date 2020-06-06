// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPatrolPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

#include "EnemyAIController.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllerPawn == nullptr)
		return EBTNodeResult::Type::Failed;
	UNavigationSystemV1* NavSyatem = UNavigationSystemV1::GetNavigationSystem(ControllerPawn->GetWorld());
	if (NavSyatem == nullptr)
		return EBTNodeResult::Type::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AEnemyAIController::HomePosKey);
	FNavLocation NextPotroal;

	bool IsOK = NavSyatem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPotroal);
	if (IsOK)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::PatrolPosKey, NextPotroal.Location);
		return EBTNodeResult::Type::Succeeded;
	}
	else
		return EBTNodeResult::Type::Failed;

	return Result;
}