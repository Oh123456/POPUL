// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyAIController.h"
#include "EnemyBaseCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	// True설정시 TickTask 호출
	bNotifyTick = true;
	m_bIsAttack = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyBaseCharacter* ControllerCharacter = Cast<AEnemyBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllerCharacter == nullptr)
		return EBTNodeResult::Failed;

	int AttackKeyValue = OwnerComp.GetBlackboardComponent()->GetValueAsInt(AEnemyAIController::AttackKey);
	Blackboard = OwnerComp.GetBlackboardComponent();
	Blackboard->SetValueAsBool(AEnemyAIController::IsAttackKey, true);
	switch (AttackKeyValue)
	{
	case AttackBranchspace::AttackBranch::E_NormalAttack:
		ControllerCharacter->Attack();
		break;
	case AttackBranchspace::AttackBranch::E_SkillAttack:
		ControllerCharacter->SkillAttack();
		break;
	case AttackBranchspace::AttackBranch::E_PlayingAttack:
		return EBTNodeResult::Succeeded;
		break;
	case AttackBranchspace::AttackBranch::E_ERROR:
		return EBTNodeResult::Succeeded;
		break;
	}
	m_bIsAttack = true;
	ControllerCharacter->OnAttackDelay.AddLambda([this]() -> void 
	{
		m_bIsAttack = false; 
		Blackboard->SetValueAsBool(AEnemyAIController::IsAttackKey, false);
	});
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!m_bIsAttack)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}