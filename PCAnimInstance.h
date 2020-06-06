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
	// �ǰ� ��Ÿ�� ����
	virtual void PlayDamageMontage(float InPlay = 1.0f) override;
	// ���� ��Ÿ�� ����
	void PlayAttackMontage(float Speed = 1.0f);
	// Ư�� ���� ��Ÿ�� ����
	void PlaySkillAttackMontage(float Speed = 1.0f);
	// ������ ��Ÿ�� ����
	void PlayRollMontage(EROLLDIRECTION Direction, float Speed = 1.0f);
	// Ư�� ���� ��Ÿ�� ����������?
	bool PlayingSkillAttackMontage();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> RollMontage;
	USoundBase* FootSound;
	
private:
	// ���� ��Ÿ�� ��Ƽ���� �Լ�
	// ������ �̵��Լ�
	// �ϴ� ����
	UFUNCTION()
		void AnimNotify_AttackEnd();
	UFUNCTION()
		void AnimNotify_Attack3MoveStart();
	UFUNCTION()
		void AnimNotify_Attack3MoveEnd();

	// ������ �߰��Է� �Լ�
	UFUNCTION()
		void AnimNotify_Additional_InputStart();
	UFUNCTION()
		void AnimNotify_Additional_InputEnd();
	//���� ������
	UFUNCTION()
		void AnimNotify_Damage_S();
	UFUNCTION()
		void AnimNotify_Damage_E();

	// ������ ��Ÿ�� ��Ƽ���� �Լ�
	UFUNCTION()
		void AnimNotify_NODamage_Start();
	UFUNCTION()
		void AnimNotify_NODamage_End();
	UFUNCTION()
		void AnimNotify_Move();

	// �߼Ҹ�
	UFUNCTION()
		void AnimNotify_Sound();

};
