// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_TurnToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BaseCharacter.h"
#include "EnemyAIController.h"

UBTTaskNode_TurnToTarget::UBTTaskNode_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseCharacter* BaseCharcter = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (BaseCharcter == nullptr)
		return EBTNodeResult::Type::Failed;
	ABaseCharacter* Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey));
	if (Target == nullptr)
		return EBTNodeResult::Type::Failed;

	FVector LookVector = Target->GetActorLocation() - BaseCharcter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	// FMath::RInterpTo 목표까지 보간하는 함수
	BaseCharcter->SetActorRotation(FMath::RInterpTo(BaseCharcter->GetActorRotation(),TargetRot,GetWorld()->GetDeltaSeconds(),2.0f));
	return EBTNodeResult::Type::Succeeded;
}