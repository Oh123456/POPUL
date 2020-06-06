// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

#include "EnemyAIController.h"
#include "BaseAnimInstance.h"
#include "POPULGameMode.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USoundWave> DefaultHit_S(TEXT("/Game/Sound/HitSound.HitSound"));
	if (DefaultHit_S.Succeeded())
	{
		HitSound = DefaultHit_S.Object;
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnBattleState.AddUObject(this, &ABaseCharacter::BattleStateChangeDeleygateFunction);
	m_bIsNoDamage = false;
	m_bIsDeath = false;
	m_bIsBattle = false;
	m_bIsMove = true;
	m_bIsRolling = false;
	m_bIsHitDamage = false;
	m_HitDelay = 1.0f;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (m_bIsNoDamage)
		return 0.0f;
	APOPULGameMode* GameMode = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	const float Damage = Super::TakeDamage(DamageAmount ,DamageEvent ,EventInstigator ,DamageCauser);

	
	
	float fDefend = (float)((float)m_Ability.nDefend * 1.125f);
	float fActualDamage = Damage - fDefend;
	if (fActualDamage <= 0.0f)
		fActualDamage = 1.0f;
	if (GameMode != nullptr && m_Ability.nHP > 0)
		GameMode->SpawnDamageTextActor(GetActorLocation(), (int)fActualDamage);
	m_Ability.nHP -= fActualDamage;
	UBaseAnimInstance* AnimInstance = Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance != nullptr)
	{
		// 몽타주에서 히트시 움직임 봉인 상태 만들기
		if (AnimInstance->IsPlaySkillAttackMontage() == false)
		{
			AnimInstance->PlayDamageMontage(m_HitDelay);
			m_bIsMove = false;
		}
	}

	if (this->GetbIsActorType() == ActorType::E_PC)
	{
		if (HitSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation((UObject*)GetWorld(), (USoundBase*)HitSound, this->GetActorLocation());
	}
	return Damage;
}


float ABaseCharacter::RandDamage()
{
	int nRnad = rand() % 40 + 80;
	float fRand = (float)((float)nRnad * 0.01f);
	float fDamage = (float)((float)m_Ability.nAttack * fRand);
	return fDamage;
}

float ABaseCharacter::RandDamage(int Range)
{
	int nRnad = rand() % Range + (100 - (Range/2));
	float fRand = (float)((float)nRnad * 0.01f);
	float fDamage = (float)((float)m_Ability.nAttack * fRand);
	return fDamage;
}

bool ABaseCharacter::GetbIsAttackRange()
{
	return false;
}

void ABaseCharacter::BattleStateChangeDeleygateFunction()
{
	GetWorld()->GetTimerManager().SetTimer(m_BattleStateTimerHandle, this, &ABaseCharacter::BattleStateChange, BATTLESTATEDURATIONTIME);
}

void ABaseCharacter::BattleStateChange()
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController != nullptr)
		AIController->GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, nullptr);
	m_bIsBattle = false;

}

void ABaseCharacter::UpDataAbility()
{
	CItem* Weapone = m_Equipment.GetEquipmentItem(EquipmentItemKinds::E_Weapone);
	CItem* Armor = m_Equipment.GetEquipmentItem(EquipmentItemKinds::E_Armor);
	float fSTR = m_OriginAbility.nSTR;
	float fDEX = m_OriginAbility.nDEX;
	int nAttack = Weapone->GetItemInformation()->ItemAbility.nAttack + m_OriginAbility.nAttack;
	int nDefend = Armor->GetItemInformation()->ItemAbility.nDefend + m_OriginAbility.nDefend;

	m_Ability.nMaxHP = m_OriginAbility.nMaxHP + (fSTR * 2 * m_OriginAbility.nMaxHP * 0.1f);
	m_Ability.nAttack = nAttack * (fSTR * 1.8f + fDEX * 0.2f) * 0.8f;
	m_Ability.nDefend = nDefend * (fSTR * 1.2f) * 0.6f;
	m_Ability.nAttackSpeed = m_OriginAbility.nAttackSpeed + fDEX * 0.001f + Weapone->GetItemInformation()->ItemAbility.nAttackSpeed;
	m_Ability.nSTR = fSTR;
	m_Ability.nDEX = fDEX;
}