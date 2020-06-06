// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseCharacter.h"
#include "DeerStagCharacter.generated.h"

/**
 * 
 */
UCLASS()
class POPUL_API ADeerStagCharacter : public AEnemyBaseCharacter
{
	GENERATED_BODY()
public:
	ADeerStagCharacter();

	// ����
	virtual bool Attack() override;
	// Ư�� ����
	virtual void SkillAttack() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// ���� üũ
	virtual void AttackHitCheck();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION()
	virtual void OnSeePawn(APawn* Pawn);
};
