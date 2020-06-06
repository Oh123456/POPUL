// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultEnemyAIController.h"
#include "Components/SkeletalMeshComponent.h"

#include "BaseCharacter.h"
#include "BaseAnimInstance.h"

using namespace AttackBranchspace;

int ADefaultEnemyAIController::AttackBranch(int Rand)
{
	int result = Super::AttackBranch(Rand);
	ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (Owner != nullptr)
	{
		UBaseAnimInstance* AnimInstance = Cast<UBaseAnimInstance>(Owner->GetMesh()->GetAnimInstance());
		if (AnimInstance != nullptr)
		{
			if (AnimInstance->IsPlaySkillAttackMontage() == true ||
				AnimInstance->IsPlayAttackMontage() == true)
				return AttackBranch::E_PlayingAttack;
		}
	}
	if (Rand > 100 || Rand < 0)
		return result;
	if (Rand <= 60)
		return AttackBranch::E_NormalAttack;
	else
		return AttackBranch::E_SkillAttack;
}