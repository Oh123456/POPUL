// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultEnemyAnimInstance.h"
#include "UObject/ConstructorHelpers.h"

#include "EnemyBaseCharacter.h"

UDefaultEnemyAnimInstance::UDefaultEnemyAnimInstance()
{

}

void UDefaultEnemyAnimInstance::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	if (!Montage_IsPlaying(AttackMontage) && !Montage_IsPlaying(HitMontage))
	{
		Montage_Play(AttackMontage);
	}
}

void UDefaultEnemyAnimInstance::PlaySkillAttackMontage()
{
	Super::PlayAttackMontage();

	if (!Montage_IsPlaying(AttackMontage) && !Montage_IsPlaying(HitMontage))
	{
		Montage_Play(SkillAttackMontage);
	}
}

void UDefaultEnemyAnimInstance::PlayDeathMontage()
{
	Super::PlayDeathMontage();
	if (!Montage_IsPlaying(DeathMontage))
	{
		Montage_Play(DeathMontage);
	}
}

void UDefaultEnemyAnimInstance::AnimNotify_Damage()
{
	AEnemyBaseCharacter* PawnOwner = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	PawnOwner->OnAttackHitCheck.Broadcast();
}

void UDefaultEnemyAnimInstance::DamageNotify()
{
	AEnemyBaseCharacter* PawnOwner = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	PawnOwner->OnAttackHitCheck.Broadcast();
}