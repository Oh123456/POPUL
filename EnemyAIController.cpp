// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine.h"

#include "BaseCharacter.h"

const FName AEnemyAIController::HomePosKey(TEXT("HomePos"));
const FName AEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyAIController::TargetKey(TEXT("Target"));
const FName AEnemyAIController::AttackKey(TEXT("AttackKey"));
const FName AEnemyAIController::IsAttackKey(TEXT("IsAttack"));

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/StagBehaviorTree.StagBehaviorTree"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/StagBlackboardData.StagBlackboardData"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> DEATHBTObject(TEXT("/Game/AI/DeathBehaviorTree.DeathBehaviorTree"));
	if (DEATHBTObject.Succeeded())
	{
		DeathBTAsset = DEATHBTObject.Object;
	}
}

void AEnemyAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	if (!UseBlackboard(BBAsset, Blackboard))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("블랙보드 없음"));
		return;
	}
	Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());


	if (!RunBehaviorTree(BTAsset))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("비헤이비어 없음"));
		return;
	}
}

void AEnemyAIController::MoveToPC()
{
	ACharacter* PlayerCharacter =  UGameplayStatics::GetPlayerCharacter((UObject*)GetWorld(), 0);
	MoveToActor((AActor*)(PlayerCharacter));
}

void AEnemyAIController::DeleteAI()
{
	if (!RunBehaviorTree(DeathBTAsset))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("비헤이비어 없음"));
		return;
	}
}
