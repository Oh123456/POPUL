// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultEnemyAnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class POPUL_API UBossAnimInstance : public UDefaultEnemyAnimInstance
{
	GENERATED_BODY()
public:
	UBossAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAnimInstance, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAnimInstance, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAnimInstance, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage4;

private:
	// 콤보 이어가기
	UFUNCTION()
		void AnimNotify_SaveAttack();
	// 콤보 리셋
	UFUNCTION()
		void AnimNotify_ResetCombo();
	UFUNCTION()
		void AnimNotify_RToL();
	// 공격중 이동 시작  
	UFUNCTION()
		void AnimNotify_Attack_Move_Strat();
	// 공격중 이동 끝
	UFUNCTION()
		void AnimNotify_Attack_Move_End();
};
