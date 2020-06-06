// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultEnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "TimerManager.h"
#include "Engine.h"

#include "EnemyAIController.h"
#include "DefaultEnemyAIController.h"
#include "EnemyAnimInstance.h"

ADefaultEnemyCharacter::ADefaultEnemyCharacter()
{
	AIControllerClass = ADefaultEnemyAIController::StaticClass();
}

void ADefaultEnemyCharacter::BeginPlay()
{
	m_AttackDelay = 1.0f;
	m_SkillAttackDelay = m_AttackDelay * 2.0f;
	m_AttackRadius = 35.0f;
	m_Ability.nAttack = 1;
	m_HitDelay = 1.0f;
	Super::BeginPlay();
	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyBaseCharacter::OnSeePawn);
	m_bIsAttack = false;
	OnAttackHitCheck.AddUObject(this, &AEnemyBaseCharacter::AttackHitCheck);
}

void ADefaultEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ADefaultEnemyCharacter::Attack()
{
	Super::Attack();
	UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnimInstance->PlayAttackMontage();
	return true;
}

void ADefaultEnemyCharacter::SkillAttack()
{
	Super::SkillAttack();
	UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnimInstance->PlaySkillAttackMontage();
}

