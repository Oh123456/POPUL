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

		//Ÿ����
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
	//FŰ ��ȣ�ۿ�
	void ActionKey();

	// ���� �̵��� �����Ƽ �ҷ�����
	void LoadAbility(Ability& Abil , Ability& OriginAbil);
	// ���� �̵��� �κ��丮	�ҷ�����
	void LoadInventory(CInventory& Inventory);
	// ���� �̵��� ������ �ҷ�����
	void LoadEquipment(CEquipment& Equipment);
	// ������
	void AddMoney(int Value);
	// �ִ� ü�� ����
	void SetMaxHP(int Value);
	// ü�� ���� 
	void AddHP(int Value);
	// �ִ� ü������
	bool IsMaxHP();
	// ��ó�� ������ Ÿ�� ã��
	int OverlapActorTypeFind();
	// OverlapActor �� EnemyBaseCharacter Ŭ�������� Ȯ��
	bool bIsOverlapActorEnemyBaseCharacter();
	// ��������
	void QuitGame();
	// ���� Ʈ���̽� �Լ�
	void AttackTraceFunction();
	void PlayerLocationToRootBoneLocation();
	// �κ��丮 ����
	void OpenInventory();
	// ����â ����
	void OpenInformation();
	// �����̺��� ������
	void QuickSave();

	inline bool GetbIsActiveInventory() { return m_bIsActiveInventory; }
	// ������ �߰�
	void AddItem(class CItem* ItemData, class UUserWidget* ItemWidget = nullptr, int Actortype = ActorType::E_PC);

	// ��ó�� ���� ����
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
	//ü���� 0�����϶�
	UFUNCTION()
		void ZeroHP();
	// UIȰ��ȭ
	void UIActive();
	// ���� ������
	void DelayJump();
	// �κ��丮 Ȱ��ȭ
	void InventoryActive(bool bIsActive);
	// ����â Ȱ��ȭ
	void InformationActive(bool bIsActive);
	// �װ� ��Ȱ���� ������
	void Deathtimer();

	// ��Ŭ��
	void LeftClickFunction();
	// ��Ŭ��
	void RightClickFunction();
	// ĳ���� �ֺ����� ������Ʈ ã��
	void FindeObject();
	// ���� Ȱ��ȭ Ű �Լ�
	void WeaponVisibilityKey();
	void EnterKey();
	// ������ Ű �Լ�
	void RollKey();
	// ������ Ű �Լ�
	void QuickSlotKey();
	void KeyDownQuitGame();
	// ���� Ű �Լ�
	void TestKey();
private:
	// ���� ������ Ÿ�̸� �Լ�
	FTimerHandle m_JumDelayTimer;
	// ��� ������ Ÿ�̸� �Լ�
	FTimerHandle m_DeathTimerHandle;

	// �ִϸ��̼� ���� ����
	// ���� �غ���
	bool m_bIsJumStrat;
	// ������
	bool m_bIsJum;
	// ���Ⲩ�����ִ� ��������
	bool m_bIsBattel;
	bool m_bIsAttack;
	// Ʈ���̽� �����ִ���
	bool m_bIstraceOn;
	bool m_bIsAdditionalOn;
	bool m_bIsAdditional_Input;

	// �κ��丮�� �����ִ���
	bool m_bIsActiveInventory;
	// ����â�� �����ִ���
	bool m_bIsActiveInmormation;
	// EscŰ�� ���ȴ���
	bool m_bIsKeyDownQuitGame;
	AActor* OverlapActor;
	// ��Ŭ�� �����
	bool m_bIsleftClicked;
	
	// �ӽ� ī��Ʈ �ڽ����� ����Ű�Է½�Ű�°� Ȯ��
	// ������
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
