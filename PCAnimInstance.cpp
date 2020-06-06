// Fill out your copyright notice in the Description page of Project Settings.

#include "PCAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "POPULCharacter.h"
#include "EnemyBaseCharacter.h"


UPCAnimInstance::UPCAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/StylizedFighter/Characters/Animations/SKL_Siete_AttackMontage.SKL_Siete_AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKEILLATTACK_MONTAGE(TEXT("/Game/StylizedFighter/Characters/Animations/Attack/A_SW_Combo_Montage.A_SW_Combo_Montage"));
	if (SKEILLATTACK_MONTAGE.Succeeded())
	{
		SkillAttackMontage = SKEILLATTACK_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_MONTAGE(TEXT("/Game/StylizedFighter/Characters/Animations/A_DamageSmall_Montage.A_DamageSmall_Montage"));
	if (HIT_MONTAGE.Succeeded())
	{
		HitMontage = HIT_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> FOOTSOUND(TEXT("/Game/FootstepSound/Sounds/Sound_Cues/MySoundCue.MySoundCue"));
	if(FOOTSOUND.Succeeded())
	{
		FootSound = FOOTSOUND.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_MONTAGE[E_ROLLMAX]
	{
		(TEXT("/Game/StylizedFighter/Characters/Animations/Roll/A_SW_Roll_Forward_Montage.A_SW_Roll_Forward_Montage")),
		(TEXT("/Game/StylizedFighter/Characters/Animations/Roll/A_SW_Roll_ForwardRight_Montage.A_SW_Roll_ForwardRight_Montage")),
		(TEXT("/Game/StylizedFighter/Characters/Animations/Roll/A_SW_Roll_Right_Montage.A_SW_Roll_Right_Montage")),
		(TEXT("/Game/StylizedFighter/Characters/Animations/Roll/A_SW_Roll_BackwardRight_Montage.A_SW_Roll_BackwardRight_Montage")),
		(TEXT("/Game/StylizedFighter/Characters/Animations/Roll/A_SW_Roll_Backward_Montage.A_SW_Roll_Backward_Montage")),
		(TEXT("/Game/StylizedFighter/Characters/Animations/Roll/A_SW_Roll_BackwardLeft_Montage.A_SW_Roll_BackwardLeft_Montage")),
		(TEXT("/Game/StylizedFighter/Characters/Animations/Roll/A_SW_Roll_Left_Montage.A_SW_Roll_Left_Montage")),
		(TEXT("/Game/StylizedFighter/Characters/Animations/Roll/A_SW_Roll_ForwardLeft_Montage.A_SW_Roll_ForwardLeft_Montage")),

	};

	for (int i = 0; i < E_ROLLMAX; i++)
	{
		if (ROLL_MONTAGE[i].Succeeded())
			RollMontage.Add(ROLL_MONTAGE[i].Object);
	}
}

void UPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	APOPULCharacter* Pawn = Cast<APOPULCharacter>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		IsJumStart = Pawn->GetIsJumStart();
		IsBattle = Pawn->GetIsBattel();
	}
}

// 몽타주 재생
void UPCAnimInstance::PlayAttackMontage(float Speed)
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, Speed);
	}
}

void UPCAnimInstance::PlaySkillAttackMontage(float Speed)
{
	if (!Montage_IsPlaying(SkillAttackMontage))
	{
		Montage_Play(SkillAttackMontage, Speed);
	}
}

void UPCAnimInstance::PlayRollMontage(EROLLDIRECTION Direction, float Speed)
{
	if (!Montage_IsPlaying(RollMontage.GetData()[Direction]))
	{
		Montage_Play(RollMontage.GetData()[Direction], Speed);
	}
}

bool UPCAnimInstance::PlayingSkillAttackMontage()
{
	return Montage_IsPlaying(SkillAttackMontage);
}

void UPCAnimInstance::PlayDamageMontage(float InPlay)
{
	if (HitMontage == nullptr)
		return;
	if (!Montage_IsPlaying(HitMontage))
	{
		if (!Montage_IsPlaying(AttackMontage))
			Montage_Play(HitMontage, InPlay);
	}
}

// 노티파이 함수 

void UPCAnimInstance::AnimNotify_AttackEnd()
{

}



void UPCAnimInstance::AnimNotify_Attack3MoveStart()
{
	auto Pawn = TryGetPawnOwner();
	APOPULCharacter* PC = Cast<APOPULCharacter>(Pawn);
	PC->AttackMove = true;
}

void UPCAnimInstance::AnimNotify_Attack3MoveEnd()
{
	auto Pawn = TryGetPawnOwner();
	APOPULCharacter* PC = Cast<APOPULCharacter>(Pawn);
	PC->AttackMove = false;
}



void UPCAnimInstance::AnimNotify_Additional_InputStart()
{
	auto Pawn = TryGetPawnOwner();
	APOPULCharacter* PC = Cast<APOPULCharacter>(Pawn);
	PC->SetAdditionalOn(true);
}

void UPCAnimInstance::AnimNotify_Additional_InputEnd()
{
	auto Pawn = TryGetPawnOwner();
	APOPULCharacter* PC = Cast<APOPULCharacter>(Pawn);
	PC->SetAdditionalOn(false);
	PC->SetbIsRolling(false);
	bool IsOk = PC->GetAdditional_Input();
	if (!IsOk)
		Montage_Stop(0.2f, AttackMontage);
	else
	{
		PC->SetAdditional_Input(false);
		APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		FRotator PCamerRoation = PlayerCameraManager->GetCameraRotation();
		FRotator PCRotation = PC->GetActorRotation();
		PCRotation.Yaw = PCamerRoation.Yaw;
		PC->SetActorRotation(PCRotation);
	}
}

void UPCAnimInstance::AnimNotify_Damage_S()
{
	auto Pawn = TryGetPawnOwner();
	APOPULCharacter* PC = Cast<APOPULCharacter>(Pawn);
	PC->SetbIstraceOn(true);
	TArray<AActor*> EActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AEnemyBaseCharacter::StaticClass(), EActor);
	for (int i = 0; i < EActor.Num(); i++)
	{
		AEnemyBaseCharacter* EnemyCharacter = Cast<AEnemyBaseCharacter>(EActor.GetData()[i]);
		if (EnemyCharacter->GetbIsHit())
			EnemyCharacter->SetbIsHit(false);
	}
}

void UPCAnimInstance::AnimNotify_Damage_E()
{
	auto Pawn = TryGetPawnOwner();
	APOPULCharacter* PC = Cast<APOPULCharacter>(Pawn);
	PC->SetbIstraceOn(false);
}

// 구르기 노티파이 함수

void UPCAnimInstance::AnimNotify_NODamage_Start()
{
	auto Pawn = TryGetPawnOwner();
	APOPULCharacter* PC = Cast<APOPULCharacter>(Pawn);
	PC->SetbIsNoDamage(true);
}

void UPCAnimInstance::AnimNotify_NODamage_End()
{
	auto Pawn = TryGetPawnOwner();
	APOPULCharacter* PC = Cast<APOPULCharacter>(Pawn);
	PC->SetbIsNoDamage(false);
}

void UPCAnimInstance::AnimNotify_Move()
{
	auto Pawn = TryGetPawnOwner();
	APOPULCharacter* PC = Cast<APOPULCharacter>(Pawn);
	PC->SetbIsMove(true);
}

void UPCAnimInstance::AnimNotify_Sound()
{
	auto Pawn = TryGetPawnOwner();
	APOPULCharacter* PC = Cast<APOPULCharacter>(Pawn);
	UGameplayStatics::PlaySoundAtLocation((UObject*)GetWorld(), FootSound, PC->GetActorLocation());
}