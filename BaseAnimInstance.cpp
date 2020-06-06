// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAnimInstance.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "BaseCharacter.h"

//void UBaseAnimInstance::PlayDamageMontage()
//{
//	if (HitMontage == nullptr)
//		return;
//	if (!Montage_IsPlaying(HitMontage))
//		Montage_Play(HitMontage);
//}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		IsInAir = Pawn->GetMovementComponent()->IsFalling();
		PawnSpeed = UKismetMathLibrary::VSize(Pawn->GetVelocity());
		
		ABaseCharacter* Character = Cast<ABaseCharacter>(Pawn);
		if (::IsValid(Character))
		{
			IsDash = Character->GetbIsDash();
			IsDeath = Character->GetbIsDeath();
		}
	}
}

void UBaseAnimInstance::PlayDamageMontage(float InPlay)
{
	if (HitMontage == nullptr)
		return;
	if (!Montage_IsPlaying(HitMontage))
		Montage_Play(HitMontage, InPlay);
}

bool UBaseAnimInstance::IsPlayAttackMontage()
{
	return Montage_IsPlaying(AttackMontage);
}

bool UBaseAnimInstance::IsPlaySkillAttackMontage()
{
	return Montage_IsPlaying(SkillAttackMontage);
}