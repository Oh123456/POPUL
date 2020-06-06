// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyAIController.h"
#include "EnemyBaseCharacter.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT(" Detect");
	Interval = 1.0f;
}


void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllerPawn == nullptr)
		return;

	UWorld* World = ControllerPawn->GetWorld();
	FVector Center = ControllerPawn->GetActorLocation();
	AEnemyBaseCharacter* ControllerCharacter = Cast<AEnemyBaseCharacter>(ControllerPawn);
	if (ControllerCharacter->GetbIsSeePlayer())
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, ControllerCharacter->GetSeePawn());
	else
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, nullptr);
	
}