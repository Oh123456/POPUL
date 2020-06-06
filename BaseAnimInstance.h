// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class POPUL_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UBaseAnimInstance() {}
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual bool IsPlayAttackMontage();
	virtual bool IsPlaySkillAttackMontage();
	virtual void PlayDamageMontage(float InPlay = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "BaseAnimInstance", meta = (BlueprintThreadSafe, DisplayName = "GetSpeed"))
		inline float GetSpeed() const { return PawnSpeed; }
	UFUNCTION(BlueprintCallable, Category = "BaseAnimInstance", meta = (BlueprintThreadSafe, DisplayName = " GetIsInAir"))
		inline bool GetIsInAir() const { return IsInAir; }
	UFUNCTION(BlueprintCallable, Category = "BaseAnimInstance", meta = (BlueprintThreadSafe, DisplayName = "GetIsJumStart"))
		inline bool GetIsJumStart() const { return IsJumStart; }
	UFUNCTION(BlueprintCallable, Category = "BaseAnimInstance", meta = (BlueprintThreadSafe, DisplayName = "GetIsBattle"))
		inline bool GetIsBattle() const { return IsBattle; }
	UFUNCTION(BlueprintCallable, Category = "BaseAnimInstance", meta = (BlueprintThreadSafe, DisplayName = "GetIsDeath"))
		inline bool GetIsDeath() const { return IsDeath; }
	UFUNCTION(BlueprintCallable, Category = "BaseAnimInstance", meta = (BlueprintThreadSafe, DisplayName = "GetIsDash"))
		inline bool GetIsDash() const { return IsDash; }
	//void PlayDamageMontage();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAnimInstance, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAnimInstance, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* SkillAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAnimInstance, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseAnimInstance, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HitMontage;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		float PawnSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		bool IsInAir;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		bool IsJumStart;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		bool IsBattle;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		bool IsDeath;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		bool IsDash;
};
