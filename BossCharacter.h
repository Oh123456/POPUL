// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultEnemyCharacter.h"
#include "BossCharacter.generated.h"

namespace BossCharacterSoace
{
	enum ERight
	{
		E_Right = true,
		E_Left	= false,
	};
};

/**
 * 
 */
UCLASS()
class POPUL_API ABossCharacter : public ADefaultEnemyCharacter
{
	GENERATED_BODY()
public:
	ABossCharacter();

	virtual void AttackHitCheck();

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	void SetbIsRigthAttck(bool BOOL) { m_bIsRigthAttck = BOOL; }
	bool GetbIsRigthAttck() const { return m_bIsRigthAttck; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	// ������ ���� ����
	bool m_bIsRigthAttck;
	// �ڽ� ��ȯ ����
	bool m_bIsBoxSpawn;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss, meta = (AllowPrivateAccess = "true"))
	// �����װ� ������ �ڽ� Ŭ����
	TSubclassOf<class ARandomBoxCharaater> SpawnBoxActor;
};
