// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAnimInstance.h"
#include "DefaultEnemyAnimInstance.generated.h"

// 기본적인 양산형 몬스터 애님인스턴스 클래스

UCLASS()
class POPUL_API UDefaultEnemyAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
public:
	UDefaultEnemyAnimInstance();

	virtual void PlayAttackMontage();
	virtual void PlaySkillAttackMontage();
	virtual void PlayDeathMontage();

	UFUNCTION(BlueprintCallable, Category = "DefaultEnemyAnimInstance", meta = (BlueprintThreadSafe, DisplayName = "DamageNotify"))
		void DamageNotify();
protected:
	UFUNCTION()
		void AnimNotify_Damage();
};
