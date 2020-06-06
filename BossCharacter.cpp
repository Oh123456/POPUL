// Fill out your copyright notice in the Description page of Project Settings.

#include "BossCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"

#include "EnemyAnimInstance.h"
#include "EnemyAIController.h"
#include "RandomBoxCharaater.h"

using namespace BossCharacterSoace;

ABossCharacter::ABossCharacter()
{
	

	static ConstructorHelpers::FClassFinder<ARandomBoxCharaater> Box(TEXT("/Game/Actor/Box/RandomBox1.RandomBox1_C"));
	if (Box.Succeeded())
	{
		SpawnBoxActor = Box.Class;
	}

}

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnAttackHitCheck.AddUObject(this, &ABossCharacter::AttackHitCheck);
	m_bIsRigthAttck = true;
	m_bIsDeath = false;
	m_bIsBoxSpawn = false;
}

void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_bIsAttackMove == true)
	{
		FVector Vector = GetActorLocation();
		SetActorLocation(Vector);
		FVector Forwardvector = GetActorForwardVector();
		Vector.X += (Forwardvector.X * 6.0f);
		Vector.Y += (Forwardvector.Y * 6.0f);
		SetActorLocation(Vector);
	}
	GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Red, UKismetStringLibrary::Conv_FloatToString(m_OriginAttckDelay));
}

void ABossCharacter::AttackHitCheck()
{
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> AttackHits;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	ActorsToIgnore.Add(this);
	FVector AttackVector(0.0f,0.0f,0.0f);
	if (m_bIsRigthAttck)
		AttackVector = GetMesh()->GetSocketLocation(FName(TEXT("R_AttackRange")));
	else
		AttackVector = GetMesh()->GetSocketLocation(FName(TEXT("L_AttackRange")));
	ABaseCharacter* HiCharacterBuffer = nullptr;
	AEnemyAIController* EnemyAIcontroller = Cast<AEnemyAIController>(GetController());
	int i = 0;

	bool bIsHitOk = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), AttackVector, AttackVector, m_AttackRadius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, AttackHits, true);
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
				m_bIsAttackHit = true;
			}
		}
	}
}

float ABossCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float const Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (m_Ability.nHP <= 0)
		if (m_bIsBoxSpawn == false)
			m_bIsDeath = true;
	if (m_bIsDeath && !m_bIsBoxSpawn)
	{
		UWorld* World = GetWorld();
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), PlayerStarts);
		//BoxSpwan
		FName SpawnTag(L"BoxSpawn");
		for (int i = 0; i < PlayerStarts.Num(); i++)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts.GetData()[i]);
			if (UKismetMathLibrary::EqualEqual_NameName(SpawnTag, PlayerStart->PlayerStartTag))
			{
				if (SpawnBoxActor != nullptr)
				{
					ARandomBoxCharaater* SpawnActor = World->SpawnActor<ARandomBoxCharaater>(SpawnBoxActor, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
					if (SpawnActor != nullptr)
						m_bIsBoxSpawn = true;
				}
			}
		}	
	}
	return Damage;
}