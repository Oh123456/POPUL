// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "CInventory.h"
#include "GameDataInstance.h"
#include "BaseCharacter.h"
#include "EnemyBaseCharacter.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheck);
DECLARE_MULTICAST_DELEGATE(FOnAttackDelay);


#define ENEMYBASEINVEN_MAX 81
//����� ���������� �ð�
#define DELETETIME 120.0f
UCLASS()
class POPUL_API AEnemyBaseCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBaseCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	// LineTrace�� Start SocketName AttakcRange_Strat, End SocketName AttakcRange_End
	virtual bool GetbIsAttackRange() override;
	// ��� true ���� ��ӹ��� �Լ����� �������ؼ� ����Ұ�
	// ���� ����
	virtual bool Attack();
	// Ư������ ����
	virtual void SkillAttack() { GetWorld()->GetTimerManager().SetTimer(m_AttackDelayHandle, this, &AEnemyBaseCharacter::AttackDelay, m_SkillAttackDelay); };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool Destroy(bool bNetForce = false, bool bShouldModifyLevel = true);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//������ �߰�
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetItem"))
		void SetItem(EItemName ItemCode, int ItemCount);
	// Ư�������� Ÿ�� �������� �߰� 
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetItemtypeAll"))
		void SetItemtypeAll(EItemtypes Itemtype, int ItemCount = 1);
	// ����ȣ ����
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetDialogNumber"))
		void SetDialogNumber(int Value) { m_nDialogNumber = (int)Value; }
	// HP ����
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetHP"))
		void SetHP(int HP) { m_Ability.nMaxHP = HP; m_Ability.nHP = HP; }
	// ���� ������ ����
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetAttackDelay"))
		void SetAttackDelay(float Value) { m_AttackDelay = Value; }
	// ���� ���� ����
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetRadius"))
		void SetRadius(float Value) { m_AttackRadius = Value; }
	// �̸� ����
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetName"))
		void SetName(FString FName) { Name = FName; }
	// �ɷ�ġ ����
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetDefaultAbility"))
		void SetDefaultAbility(FString FName = TEXT(" "), int HP = 0, float AttackDelay = -1, float SKillAttackDelay = -1, float AttackRadius = 35.0f, float HitDelay = 0.8f, int Attack = -1, int Defend = 0);
	
	UFUNCTION()
		void AttackDelay() { m_bIsAttack = false; OnAttackDelay.Broadcast(); }
	// ���� SeeePawn���� �÷��̾ ������ �󸶳� ���������� �÷��̾ ������ �����ϴ� �Լ�
	UFUNCTION()
		void bIsSeePlayerTimer() { if (!m_bIsBattle) m_bIsSeePlayer = false; }
	UFUNCTION()
		void OnSeePawn(APawn* Pawn);
	// Ÿ������Ȯ��
	UFUNCTION()
		void AttackHitCheck();
	FString GetNaem() { return Name; }
	APawn* GetSeePawn() { return SeePawn; }
	int GetDialogNumber() { return m_nDialogNumber; }
	bool GetbIsSeePlayer() { return m_bIsSeePlayer; }

	void SetAttackMove(bool BOOL) { m_bIsAttackMove = BOOL; }
	bool GetAttackMove() const { return m_bIsAttackMove; }

	void SetbIsAttackHit(bool BOOL) { m_bIsAttackHit = BOOL; }
	bool GetbIsAttackHit() const { return m_bIsAttackHit; }
	
private:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	inline void DeleteCharacter();
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UPawnSensingComponent* PawnSensing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* HPWidget;

	FOnAttackHitCheck OnAttackHitCheck;
	FOnAttackDelay OnAttackDelay;

	
protected:
	// �̸� 
	FString Name;
	// ����� Pawn
	APawn* SeePawn;
	// �������� ���� ������ 
	float m_AttackDelay;
	// �⺻ ���� ������
	float m_OriginAttckDelay;
	// Ư�� ���� ������
	float m_SkillAttackDelay;
	// ���� ���� 
	float m_AttackRadius;
	// ���� ����
	bool m_bIsAttack;
	// �÷��̾ �����ִ��� 
	bool m_bIsSeePlayer;
	// ������ �̵��ϴ���
	bool m_bIsAttackMove;
	// �ǰ݉����
	bool m_bIsAttackHit;

	// ���ݵ����� Ÿ�̸��ڵ�
	FTimerHandle m_AttackDelayHandle;
	// �÷��̾ �󸶳� ������ �����ϴ� Ÿ�̸��ڵ�
	FTimerHandle m_SeePlayerHandle;
	// ĳ���� ���� Ÿ�̸� �ڵ�
	FTimerHandle m_DeleteCharacterHandle;

private:
	class UDefualtUserWidget* DefaultUIBuffer;
	class APOPULCharacter* CharacterBuffer;

	// ��� ����
	int m_nDialogNumber;
};
