// Fill out your copyright notice in the Description page of Project Settings.


#include "DeerStagAnimInstance.h"
#include "UObject/ConstructorHelpers.h"

#include "POPUL.h"
#include "EnemyBaseCharacter.h"


UDeerStagAnimInstance::UDeerStagAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/AnimalVarietyPack/DeerStagAndDoe/Animations/ANIM_DeerStag_AntlersAttack_Montage.ANIM_DeerStag_AntlersAttack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKEILLATTACK_MONTAGE(TEXT("/Game/AnimalVarietyPack/DeerStagAndDoe/Animations/ANIM_DeerStag_AntlersComboAttack_Montage.ANIM_DeerStag_AntlersComboAttack_Montage"));
	if (SKEILLATTACK_MONTAGE.Succeeded())
	{
		SkillAttackMontage = SKEILLATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_MONTAGE(TEXT("/Game/AnimalVarietyPack/DeerStagAndDoe/Animations/ANIM_DeerStag_GetHit_Montage.ANIM_DeerStag_GetHit_Montage"));
	if (HIT_MONTAGE.Succeeded())
	{
		HitMontage = HIT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT("/Game/AnimalVarietyPack/DeerStagAndDoe/Animations/ANIM_DeerStag_Death_Montage.ANIM_DeerStag_Death_Montage"));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}
}

void UDeerStagAnimInstance::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	if (!Montage_IsPlaying(AttackMontage) && !Montage_IsPlaying(HitMontage))
	{
		Montage_Play(AttackMontage);
	}
}


void UDeerStagAnimInstance::PlaySkillAttackMontage()
{
	Super::PlayAttackMontage();

	if (!Montage_IsPlaying(AttackMontage) && !Montage_IsPlaying(HitMontage))
	{
		Montage_Play(SkillAttackMontage);
	}
}

void UDeerStagAnimInstance::PlayDeathMontage()
{
	Super::PlayDeathMontage();
	if (!Montage_IsPlaying(DeathMontage))
	{
		Montage_Play(DeathMontage);
	}
}

void UDeerStagAnimInstance::AnimNotify_Damage_Start()
{
	AEnemyBaseCharacter* PawnOwner = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	PawnOwner->OnAttackHitCheck.Broadcast();
}

void UDeerStagAnimInstance::AnimNotify_Death()
{
	//UE_LOG(POPUL, Warning, TEXT("asdfasdf"));
}


void UDeerStagAnimInstance::AnimNotify_Move_Start()
{
	auto Pawn = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	Pawn->SetAttackMove(true);
}

void UDeerStagAnimInstance::AnimNotify_Move_End()
{
	auto Pawn = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	Pawn->SetAttackMove(false);
}

void UDeerStagAnimInstance::AnimNotify_Move()
{
	auto Pawn = TryGetPawnOwner();
	AEnemyBaseCharacter* PC = Cast<AEnemyBaseCharacter>(Pawn);
	PC->SetbIsMove(true);
}