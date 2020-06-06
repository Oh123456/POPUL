// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */

namespace AttackBranchspace
{
	enum AttackBranch
	{
		E_PlayingAttack = -2,
		E_ERROR = -1,
		E_NormalAttack = 0,
		E_SkillAttack = 1,
	};

}

UCLASS()
class POPUL_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();
	// ��� ���� 
	void MoveToPC();
	virtual void Possess(APawn* InPawn) override;
	// ���� �б� �Լ� ��ӹ޾Ƽ� ���͸��� ����Ұ�
	virtual int AttackBranch(int Rand) { return AttackBranchspace::AttackBranch::E_NormalAttack; };
	inline void DeleteAI();
public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName AttackKey;
	static const FName IsAttackKey;

private:
	// �⺻ Ʈ�������̺��
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	// ������ 
	UPROPERTY()
		class UBlackboardData* BBAsset;
	// ����� ����� Ʈ�������̺��
	UPROPERTY()
		class UBehaviorTree* DeathBTAsset;
};
