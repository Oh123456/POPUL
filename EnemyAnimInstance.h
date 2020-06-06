// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class POPUL_API UEnemyAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UEnemyAnimInstance();

	// 공격 몽타주 실행
	virtual void PlayAttackMontage() {};
	// 특수 공격 몽타주 실행
	virtual void PlaySkillAttackMontage() {};
	// 사용 안함
	virtual void PlayDeathMontage() {};

	
};
