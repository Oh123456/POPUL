// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerStagCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "TimerManager.h"
#include "Engine.h"

#include "EnemyAIController.h"
#include "DefaultEnemyAIController.h"
#include "EnemyAnimInstance.h"

ADeerStagCharacter::ADeerStagCharacter()
{
	AIControllerClass = ADefaultEnemyAIController::StaticClass();
}

void ADeerStagCharacter::BeginPlay()
{
	m_AttackDelay = 1.5f;
	m_SkillAttackDelay = m_AttackDelay * 2.0f;
	m_AttackRadius = 35.0f;
	m_Ability.nAttack = 50;
	m_HitDelay = 0.8f;
	// Super::BeginPlay는 블루프린트에서 설정한것을 불러오기때문에 기본이되는 값들은 Super::BeginPlay보다 위에 작성을 해둬야한다
	Super::BeginPlay();
	PawnSensing->OnSeePawn.AddDynamic(this, &ADeerStagCharacter::OnSeePawn);
	m_bIsAttack = false;
	OnAttackHitCheck.AddUObject(this,&ADeerStagCharacter::AttackHitCheck);
}

void ADeerStagCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_bIsAttackMove)
	{
		FVector Vector = GetActorLocation();
		SetActorLocation(Vector);
		FVector Forwardvector = GetActorForwardVector();
		Vector.X += (Forwardvector.X * 1.0f);
		Vector.Y += (Forwardvector.Y * 1.0f);
		SetActorLocation(Vector);
	}
}

bool ADeerStagCharacter::Attack()
{
	Super::Attack();
	UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnimInstance->PlayAttackMontage();
	return true;
	
}

void ADeerStagCharacter::SkillAttack()
{
	Super::SkillAttack();
	UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnimInstance->PlaySkillAttackMontage();
}

void ADeerStagCharacter::OnSeePawn(APawn* Pawn)
{
	float SensingInterval = PawnSensing->SensingInterval;

	AEnemyAIController* EAIcontroller = Cast<AEnemyAIController>(GetController());
	USkeletalMeshComponent* Mesh = GetMesh();
	UWorld* World = GetWorld();

	if (m_Ability.nHP <= 0)
		return;
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,TEXT("OnSeePawn"));

	m_bIsSeePlayer = true;
	SeePawn = Pawn;
	if (m_bIsBattle)
	{
		m_bIsBattle = true;
		OnBattleState.Broadcast();
	}
	World->GetTimerManager().SetTimer(m_SeePlayerHandle,this, &AEnemyBaseCharacter::bIsSeePlayerTimer, SensingInterval + 0.1f);
	//World->GetTimerManager().SetTimer(m_AttackDelayHandle, this, &AEnemyBaseCharacter::AttackDelay, m_AttackDelay);

	//if (m_bIsAttack)
	//	return; 

	//TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	//TArray<AActor*> ActorsToIgnore;
	//TArray<FHitResult> OutHits;
	//ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	//ActorsToIgnore.Add(this);
	//FVector Start = Mesh->GetBoneLocation(FName(TEXT("STAG_-Head")));
	//FVector End = Mesh->GetSocketLocation(FName(TEXT("FinePlayer")));
	//int i = 0;
	//bool bIsPC = false;

	//bool bIsOk = UKismetSystemLibrary::LineTraceMultiForObjects(World, Start, End, ObjectTypes, false , ActorsToIgnore, EDrawDebugTrace::ForOneFrame , OutHits, true);
	//if (bIsOk)
	//{
	//	for (i = 0; i < OutHits.Num(); i++)
	//	{
	//		ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(OutHits.GetData()[i].GetActor());
	//		if (HitCharacter->GetbIsActorType() == ActorType::E_PC)
	//		{
	//			bIsPC = true;
	//			break;
	//		}
	//		bIsPC = false;
	//	}
	//	if (!bIsPC)
	//		return;

	//	UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(Mesh->GetAnimInstance());
	//	EnemyAnimInstance->PlayAttackMontage();
	//	m_bIsAttack = true;
	//	World->GetTimerManager().SetTimer(m_AttackDelayHandle, this, &AEnemyBaseCharacter::AttackDelay, m_AttackDelay);

	//	ABaseCharacter* Buffer = nullptr;
	//	TArray<FHitResult> AttackHits;
	//	FVector AttackVector = Start;
	//	bool bIsHitOk = UKismetSystemLibrary::SphereTraceMultiForObjects(World, AttackVector, AttackVector, m_AttackRadius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, AttackHits, true);
	//	for (i = 0; i < AttackHits.Num(); i++)
	//	{
	//		ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(AttackHits.GetData()[i].GetActor());
	//		if (HitCharacter->GetbIsActorType() == ActorType::E_PC)
	//		{
	//			if (Buffer != nullptr)
	//			{
	//				if (Buffer == HitCharacter)
	//					continue;
	//			}
	//			float fDamage = RandDamage();
	//			UGameplayStatics::ApplyDamage(HitCharacter, fDamage, EAIcontroller, this, UDamageType::StaticClass());
	//			Buffer = HitCharacter;
	//		}
	//	}
	//}
}


void ADeerStagCharacter::AttackHitCheck()
{
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> AttackHits;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	ActorsToIgnore.Add(this);
	FVector AttackVector = GetMesh()->GetSocketLocation(FName(TEXT("AttackRange_Start")));
	ABaseCharacter* HiCharacterBuffer = nullptr;
	AEnemyAIController* EnemyAIcontroller = Cast<AEnemyAIController>(GetController());
	int i = 0;

	bool bIsHitOk = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), AttackVector, AttackVector, m_AttackRadius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, AttackHits, true);
	if (bIsHitOk)
	{
		for (i = 0; i < AttackHits.Num(); i++)
		{
			ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(AttackHits.GetData()[i].GetActor());
			if (HitCharacter == nullptr)
				continue;
			if (HitCharacter->GetbIsActorType() == ActorType::E_PC)
			{
				if (HiCharacterBuffer != nullptr)
				{
					if (HiCharacterBuffer == HitCharacter)
						continue;
				}
				float fDamage = RandDamage();
				UGameplayStatics::ApplyDamage(HitCharacter, fDamage, EnemyAIcontroller, this, UDamageType::StaticClass());
				HiCharacterBuffer = HitCharacter;
			}
		}
	}
}