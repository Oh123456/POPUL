// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_AttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyAIController.h"
#include "EnemyBaseCharacter.h"

UBTDecorator_AttackRange::UBTDecorator_AttackRange()
{
	NodeName = TEXT("IsAttack");
}

bool UBTDecorator_AttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ABaseCharacter* ControllPawn = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllPawn == nullptr)
		return false;
	auto Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetKey);
	if (Target == nullptr)
		return false;

	bResult = ControllPawn->GetbIsAttackRange();
	return bResult;
}