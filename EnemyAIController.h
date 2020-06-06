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
	// 사용 안함 
	void MoveToPC();
	virtual void Possess(APawn* InPawn) override;
	// 공격 분기 함수 상속받아서 몬스터마다 사용할것
	virtual int AttackBranch(int Rand) { return AttackBranchspace::AttackBranch::E_NormalAttack; };
	inline void DeleteAI();
public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName AttackKey;
	static const FName IsAttackKey;

private:
	// 기본 트리비헤이비어
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	// 블랙보드 
	UPROPERTY()
		class UBlackboardData* BBAsset;
	// 사망시 사용할 트리비헤이비어
	UPROPERTY()
		class UBehaviorTree* DeathBTAsset;
};
