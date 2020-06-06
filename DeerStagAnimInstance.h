// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAnimInstance.h"
#include "DeerStagAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class POPUL_API UDeerStagAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
public:
	UDeerStagAnimInstance();

	virtual void PlayAttackMontage();
	virtual void PlaySkillAttackMontage();
	virtual void PlayDeathMontage();
private:
	// 타격 지점
	UFUNCTION()
		void AnimNotify_Damage_Start();
	UFUNCTION()
		void AnimNotify_Death();
	// 공격중 이동함수 시작
	UFUNCTION()
		void AnimNotify_Move_Start();
	// 공격중 이동함수 끝
	UFUNCTION()
		void AnimNotify_Move_End();
	UFUNCTION()
		void AnimNotify_Move();

};
