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
//사망후 삭제까지의 시간
#define DELETETIME 120.0f
UCLASS()
class POPUL_API AEnemyBaseCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBaseCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	// LineTrace의 Start SocketName AttakcRange_Strat, End SocketName AttakcRange_End
	virtual bool GetbIsAttackRange() override;
	// 상시 true 리턴 상속받은 함수에서 재정의해서 사용할것
	// 공격 실행
	virtual bool Attack();
	// 특수공격 실핼
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

	//아이템 추가
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetItem"))
		void SetItem(EItemName ItemCode, int ItemCount);
	// 특정아이템 타입 모든아이템 추가 
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetItemtypeAll"))
		void SetItemtypeAll(EItemtypes Itemtype, int ItemCount = 1);
	// 대사번호 설정
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetDialogNumber"))
		void SetDialogNumber(int Value) { m_nDialogNumber = (int)Value; }
	// HP 설정
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetHP"))
		void SetHP(int HP) { m_Ability.nMaxHP = HP; m_Ability.nHP = HP; }
	// 공격 딜레이 설정
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetAttackDelay"))
		void SetAttackDelay(float Value) { m_AttackDelay = Value; }
	// 공격 범위 설정
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetRadius"))
		void SetRadius(float Value) { m_AttackRadius = Value; }
	// 이름 설정
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetName"))
		void SetName(FString FName) { Name = FName; }
	// 능력치 설정
	UFUNCTION(BlueprintCallable, Category = "EnemyBaseCharacter", meta = (DisplayName = "SetDefaultAbility"))
		void SetDefaultAbility(FString FName = TEXT(" "), int HP = 0, float AttackDelay = -1, float SKillAttackDelay = -1, float AttackRadius = 35.0f, float HitDelay = 0.8f, int Attack = -1, int Defend = 0);
	
	UFUNCTION()
		void AttackDelay() { m_bIsAttack = false; OnAttackDelay.Broadcast(); }
	// 센서 SeeePawn으로 플레이어를 봤을때 얼마나 지속적으로 플레이어를 보는지 설정하는 함수
	UFUNCTION()
		void bIsSeePlayerTimer() { if (!m_bIsBattle) m_bIsSeePlayer = false; }
	UFUNCTION()
		void OnSeePawn(APawn* Pawn);
	// 타격판정확인
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
	// 이름 
	FString Name;
	// 목격한 Pawn
	APawn* SeePawn;
	// 실직적인 공격 딜레이 
	float m_AttackDelay;
	// 기본 공격 딜레이
	float m_OriginAttckDelay;
	// 특수 공격 딜레이
	float m_SkillAttackDelay;
	// 공격 범위 
	float m_AttackRadius;
	// 공격 여부
	bool m_bIsAttack;
	// 플레이어를 보고있는지 
	bool m_bIsSeePlayer;
	// 공격중 이동하는지
	bool m_bIsAttackMove;
	// 피격됬는지
	bool m_bIsAttackHit;

	// 공격딜레이 타이머핸들
	FTimerHandle m_AttackDelayHandle;
	// 플레이어를 얼마나 보는지 설정하는 타이머핸들
	FTimerHandle m_SeePlayerHandle;
	// 캐릭터 삭제 타이머 핸들
	FTimerHandle m_DeleteCharacterHandle;

private:
	class UDefualtUserWidget* DefaultUIBuffer;
	class APOPULCharacter* CharacterBuffer;

	// 대사 숫자
	int m_nDialogNumber;
};
