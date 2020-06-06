// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnimInstance.h"

#include "BossCharacter.h"
//#include "EnemyBaseCharacter.h"

UBossAnimInstance::UBossAnimInstance()
{

}


void UBossAnimInstance::AnimNotify_SaveAttack()
{
	ABossCharacter* Owner = Cast<ABossCharacter>(TryGetPawnOwner());
	if (Owner != nullptr)
	{
		if (Owner->GetbIsAttackHit())
		{
			if (Montage_IsPlaying(AttackMontage))
			{
				Montage_Play(AttackMontage2);
				Owner->SetbIsRigthAttck(false);
			}
			else if (Montage_IsPlaying(AttackMontage2))
			{
				Montage_Play(AttackMontage3);
				Owner->SetbIsRigthAttck(true);
			}
			else if (Montage_IsPlaying(AttackMontage3))
			{
				Montage_Play(AttackMontage4);
				Owner->SetbIsRigthAttck(false);
			}
			Owner->SetbIsAttackHit(false);
		}
	}
}

void UBossAnimInstance::AnimNotify_ResetCombo()
{
	ABossCharacter* Owner = Cast<ABossCharacter>(TryGetPawnOwner());
	if (Owner != nullptr)
		Owner->SetbIsRigthAttck(true);
}

void UBossAnimInstance::AnimNotify_RToL()
{
	ABossCharacter* Owner = Cast<ABossCharacter>(TryGetPawnOwner());
	if (Owner != nullptr)
		Owner->SetbIsRigthAttck(false);
}

void UBossAnimInstance::AnimNotify_Attack_Move_Strat()
{
	ABossCharacter* Owner = Cast<ABossCharacter>(TryGetPawnOwner());
	if (Owner != nullptr)
		Owner->SetAttackMove(true);
}

void UBossAnimInstance::AnimNotify_Attack_Move_End()
{
	ABossCharacter* Owner = Cast<ABossCharacter>(TryGetPawnOwner());
	if (Owner != nullptr)
		Owner->SetAttackMove(false);
}
