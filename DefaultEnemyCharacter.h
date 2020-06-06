// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseCharacter.h"
#include "DefaultEnemyCharacter.generated.h"

// 기본적인 양산형 몬스터 클래스

UCLASS()
class POPUL_API ADefaultEnemyCharacter : public AEnemyBaseCharacter
{
	GENERATED_BODY()
public:
	ADefaultEnemyCharacter();

	virtual bool Attack() override;
	virtual void SkillAttack() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
