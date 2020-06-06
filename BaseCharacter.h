// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActortypeInterface.h"
#include "CInventory.h"
#include "CEquipment.h"
#include "BaseCharacter.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnBattleState);

// 전투상태 지속 시간
#define BATTLESTATEDURATIONTIME 12.0f
UCLASS()
class POPUL_API ABaseCharacter : public ACharacter, public IActortypeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// 상시 false 리턴 상속받은 클래스에서 재정이해서 사용할것 비공격 유닛일경우정의 할필요 없다.
	virtual bool GetbIsAttackRange();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "BaseCharacter", meta = (DisplayName = "SetAbility"))
		void SetAbility(FAbility AbilityData) { m_Ability = AbilityData; }
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter", meta = (DisplayName = "SetDash"))
		void SetbIsDash(bool BOOL) { m_bIsDash = BOOL; }
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter", meta = (DisplayName = "SetbIsDeath"))
		void SetbIsDeath(bool BOOL) { m_bIsDeath = BOOL; }

	UFUNCTION(BlueprintCallable, Category = "BaseCharacter", meta = (DisplayName = "GetHPTest"))
		int GetHPTest() { return m_Ability.nHP ; }
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter", meta = (DisplayName = "GetDash"))
		bool GetbIsDash() { return m_bIsDash; }
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter", meta = (DisplayName = "GetbIsDeath"))
		bool GetbIsDeath() { return m_bIsDeath; }
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
public:
	void UpDataAbility();

	void SetbIsHit(bool BOOL) { m_bIshit = BOOL; }
	void SetbIsNoDamage(bool BOOL) { m_bIsNoDamage = BOOL; }
	void SetbIsMove(bool BOOL) { m_bIsMove = BOOL; }
	void SetbIsRolling(bool BOOL) { m_bIsRolling = BOOL; }

	bool GetbIsRolling() const { return m_bIsRolling; }
	bool GetbIsHit() { return m_bIshit; }
	bool GetbIsNoDamage() { return m_bIsNoDamage; }
	Ability* GetAbility() { return &m_Ability; }
	Ability* GetOriginAbility() { return &m_OriginAbility; }
	CEquipment* GetEquipmet() { return &m_Equipment; }
	CInventory* GetInventory() { return &m_MyInventory; }
public:
	FOnBattleState OnBattleState;
protected:
	float RandDamage();
	float RandDamage(int Range);
	// 전투상태에서 비전투 상태로 변경
	void BattleStateChange();
	void BattleStateChangeDeleygateFunction();
protected:
	FTimerHandle m_BattleStateTimerHandle;
	Ability m_Ability;
	Ability m_OriginAbility;
	CEquipment m_Equipment;
	CInventory m_MyInventory;
	float m_HitDelay;
	bool m_bIshit;
	bool m_bIsDash;
	bool m_bIsNoDamage;
	bool m_bIsDeath;
	bool m_bIsBattle;
	bool m_bIsMove;
	bool m_bIsRolling;
	bool m_bIsHitDamage;

	// 플에이어 피격사운드 즉 몬스터의 타격사운드이다
	// 난중에 사운드매니저 클레스 생성해서 몬스터마다 사운드를 조정
	// 기본 피격사운드는 둔탁한 타격음이다
	class USoundWave* HitSound;
};
