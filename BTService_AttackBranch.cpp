// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_AttackBranch.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyAIController.h"

UBTService_AttackBranch::UBTService_AttackBranch()
{
	NodeName = TEXT("AttackBranch");
	Interval = 1.0f;
}

void UBTService_AttackBranch::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	int Rnad = rand() % 100;

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()->GetPawn()->GetController());
	int Result = AIController->AttackBranch(Rnad);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyAIController::AttackKey, Result);
}