// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "CInventory.h"
#include "BaseCharacter.h"
#include "POPULCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMoneyChangedDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryActive, bool);

UCLASS(config = Game)
class APOPULCharacter : public ABaseCharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
			class USpringArmComponent* CameraBoom;

		/** Follow camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
			class UCameraComponent* FollowCamera;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
			class USkeletalMeshComponent* WeponMesh;

		//타격음
		class USoundWave* BlowSound;
public:
	APOPULCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
public:
	UFUNCTION(BlueprintCallable, Category = "POPULCharacter")
		inline  bool GetIsJumStart() { return m_bIsJumStrat; }
	UFUNCTION(BlueprintCallable, Category = "POPULCharacter")
		inline  bool GetIsBattel() { return m_bIsBattel; }

	inline  void SetIsBattel(bool BOOL) { m_bIsBattel = BOOL; }
	void SetWeaponVisibility(bool BOOL);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	bool Destroy(bool bNetForce = false, bool bShouldModifyLevel = true);
	//F키 상호작용
	void ActionKey();

	// 레벨 이동시 어빌리티 불러오기
	void LoadAbility(Ability& Abil , Ability& OriginAbil);
	// 레벨 이동시 인벤토리	불러오기
	void LoadInventory(CInventory& Inventory);
	// 레벨 이동시 장착템 불러오기
	void LoadEquipment(CEquipment& Equipment);
	// 돈증감
	void AddMoney(int Value);
	// 최대 체력 세팅
	void SetMaxHP(int Value);
	// 체력 증감 
	void AddHP(int Value);
	// 최대 체력인지
	bool IsMaxHP();
	// 곂처진 액터의 타입 찾기
	int OverlapActorTypeFind();
	// OverlapActor 가 EnemyBaseCharacter 클레스인지 확인
	bool bIsOverlapActorEnemyBaseCharacter();
	// 게임종료
	void QuitGame();
	// 어택 트레이스 함수
	void AttackTraceFunction();
	void PlayerLocationToRootBoneLocation();
	// 인벤토리 열기
	void OpenInventory();
	// 정보창 열기
	void OpenInformation();
	// 퀵세이브기능 사용안함
	void QuickSave();

	inline bool GetbIsActiveInventory() { return m_bIsActiveInventory; }
	// 아이템 추가
	void AddItem(class CItem* ItemData, class UUserWidget* ItemWidget = nullptr, int Actortype = ActorType::E_PC);

	// 곂처진 액터 설정
	void SetOverlapActor(AActor* Actor);
	void SetbIsKeyDownQuitGame(bool Bool) { m_bIsKeyDownQuitGame = Bool; }
	void SetAdditionalOn(bool BOOL) { m_bIsAdditionalOn = BOOL; }
	void SetAdditional_Input(bool BOOL) { m_bIsAdditional_Input = BOOL; }
	void SetbIstraceOn(bool BOOL) { m_bIstraceOn = BOOL; }
	void SetbIsCountVisibility(bool BOOL) { bIsCountVisibility = BOOL; }
	void SetCountBoxUICameraRotation();

	AActor* GetOverlapActor() { return OverlapActor; }
	bool GetbIsLeftChilcked() { return m_bIsleftClicked; }
	bool GetbIsKeyDownQuitGame() { return m_bIsKeyDownQuitGame; }
	bool GetAdditionalOn() { return m_bIsAdditionalOn; }
	bool GetAdditional_Input() { return m_bIsAdditional_Input; }
	bool GetbIstraceOn() { return m_bIstraceOn; }
private:
	//체력이 0이하일때
	UFUNCTION()
		void ZeroHP();
	// UI활성화
	void UIActive();
	// 점프 딜레이
	void DelayJump();
	// 인벤토리 활성화
	void InventoryActive(bool bIsActive);
	// 정보창 활성화
	void InformationActive(bool bIsActive);
	// 죽고 부활까지 딜레이
	void Deathtimer();

	// 좌클릭
	void LeftClickFunction();
	// 우클릭
	void RightClickFunction();
	// 캐릭터 주변으로 오브젝트 찾기
	void FindeObject();
	// 무기 활성화 키 함수
	void WeaponVisibilityKey();
	void EnterKey();
	// 구르기 키 함수
	void RollKey();
	// 퀵슬롯 키 함수
	void QuickSlotKey();
	void KeyDownQuitGame();
	// 실험 키 함수
	void TestKey();
private:
	// 점프 딜레이 타이머 함수
	FTimerHandle m_JumDelayTimer;
	// 사망 딜레이 타이머 함수
	FTimerHandle m_DeathTimerHandle;

	// 애니메이션 관련 변수
	// 점프 준비동작
	bool m_bIsJumStrat;
	// 점프중
	bool m_bIsJum;
	// 무기꺼내져있는 상태인지
	bool m_bIsBattel;
	bool m_bIsAttack;
	// 트레이스 켜져있느지
	bool m_bIstraceOn;
	bool m_bIsAdditionalOn;
	bool m_bIsAdditional_Input;

	// 인벤토리가 열려있는지
	bool m_bIsActiveInventory;
	// 정보창이 열려있는지
	bool m_bIsActiveInmormation;
	// Esc키가 눌렸는지
	bool m_bIsKeyDownQuitGame;
	AActor* OverlapActor;
	// 좌클릭 됬는지
	bool m_bIsleftClicked;
	
	// 임시 카운트 박스에서 엔터키입력시키는거 확인
	// 실패함
	bool bIsCountVisibility;
	FRotator CountBoxUICameraRotation;
public:
	bool AttackMove;
	static bool bIsMoveLevel;
	static int InvenMax;
public:
	FOnHPChangedDelegate OnHPChange;
	FOnInventoryActive OnInventoryActive;
	FOnInventoryActive OnInformationActive;
	FOnMoneyChangedDelegate OnMoneyChange;
};
//UKismetSystemLibrary::
