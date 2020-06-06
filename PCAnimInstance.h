// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
//#include "Sound/SoundCue.h"
#include "PCAnimInstance.generated.h"

/**
 * 
 */
enum EROLLDIRECTION
{
	E_FORWARD			= 0,
	E_FORWARDRIAHT		= 1,
	E_RIAHT				= 2,
	E_BACKWARDRIAHT		= 3,
	E_BACKWARD			= 4,
	E_BACKWARDLEFT		= 5,
	E_LEFT				= 6,
	E_FORWARDLEFT		= 7,
	E_ROLLMAX			= 8
};

UCLASS()
class POPUL_API UPCAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
public:
	UPCAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	// 피격 몽타주 실행
	virtual void PlayDamageMontage(float InPlay = 1.0f) override;
	// 공격 몽타주 실행
	void PlayAttackMontage(float Speed = 1.0f);
	// 특수 공격 몽타주 실행
	void PlaySkillAttackMontage(float Speed = 1.0f);
	// 구르기 몽타주 실행
	void PlayRollMontage(EROLLDIRECTION Direction, float Speed = 1.0f);
	// 특수 공격 몽타주 실행중인지?
	bool PlayingSkillAttackMontage();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> RollMontage;
	USoundBase* FootSound;
	
private:
	// 공격 몽타주 노티파이 함수
	// 공격중 이동함수
	// 일단 페지
	UFUNCTION()
		void AnimNotify_AttackEnd();
	UFUNCTION()
		void AnimNotify_Attack3MoveStart();
	UFUNCTION()
		void AnimNotify_Attack3MoveEnd();

	// 공격중 추가입력 함수
	UFUNCTION()
		void AnimNotify_Additional_InputStart();
	UFUNCTION()
		void AnimNotify_Additional_InputEnd();
	//공격 데미지
	UFUNCTION()
		void AnimNotify_Damage_S();
	UFUNCTION()
		void AnimNotify_Damage_E();

	// 구르기 몽타주 노티파이 함수
	UFUNCTION()
		void AnimNotify_NODamage_Start();
	UFUNCTION()
		void AnimNotify_NODamage_End();
	UFUNCTION()
		void AnimNotify_Move();

	// 발소리
	UFUNCTION()
		void AnimNotify_Sound();

};
