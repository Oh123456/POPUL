// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/DamageType.h"

#include "POPUL.h"
#include "GameDataInstance.h"
#include "POPULCharacter.h"
#include "POPULGameMode.h"
#include "MainHUD.h"
#include "DefualtUserWidget.h"
#include "InventorySlotUserWidget.h"
#include "MyAIController.h"
#include "EnemyAnimInstance.h"
#include "EnemyAIController.h"
#include "DefaultEnemyAIController.h"
#include "OKBoxUserWidget.h"
#include "HPUserWidget.h"

// Sets default values
AEnemyBaseCharacter::AEnemyBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment((USceneComponent*)GetMesh());
	BoxComponent->SetGenerateOverlapEvents(true);

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
	HPWidget->SetupAttachment(GetMesh());
	HPWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> HP(TEXT("/Game/Widget/HPWidgetBlueprint.HPWidgetBlueprint_C"));
	if (HP.Succeeded())
	{
		HPWidget->SetWidgetClass(HP.Class);
		HPWidget->SetDrawSize(FVector2D(200.0f, 25.0f));
	}
}

// Called when the game starts or when spawned
void AEnemyBaseCharacter::BeginPlay()
{
	m_MyInventory.SetMaxInventoryItem(ENEMYBASEINVEN_MAX);
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBaseCharacter::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemyBaseCharacter::OnEndOverlap);
	bIsActorType = ActorType::E_Enemy;
	m_bIsSeePlayer = false;
	m_bIsAttackHit = false;

}

// Called every frame
void AEnemyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBaseCharacter::SetItem(EItemName ItemCode, int ItemCount)
{
	APOPULGameMode* GameMode = (APOPULGameMode*)UGameplayStatics::GetGameMode((UObject*)GetWorld());
	CItem Items = GameMode->GetItemData((int32)ItemCode);
	Items.GetItemInformation()->nCount = ItemCount;
	AEnemyBaseCharacter::m_MyInventory.AddItem(&Items, nullptr);
}

void AEnemyBaseCharacter::SetItemtypeAll(EItemtypes Itemtype, int ItemCount)
{
	APOPULGameMode* GameMode = (APOPULGameMode*)UGameplayStatics::GetGameMode((UObject*)GetWorld());
	for (int i = 0; i < (int)EItemName::E_ITEMNAMEMAX; i++)
	{
		CItem Items = GameMode->GetItemData(i);
		if (Items.GetItemInformation()->Itemtype == (int)Itemtype)
		{
			Items.GetItemInformation()->nCount = ItemCount;
			AEnemyBaseCharacter::m_MyInventory.AddItem(&Items, nullptr, ActorType::E_NPC);
		}
	}
}

bool AEnemyBaseCharacter::Destroy(bool bNetForce, bool bShouldModifyLevel)
{
	bool pendingkillpednding = Super::Destroy();
	if (!pendingkillpednding)
		m_MyInventory.Delete();
	return pendingkillpednding;
}

// Called to bind functionality to input
void AEnemyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (bIsActorType == ActorType::E_NPC)
		return Result;
	if (m_bIsDeath)
		return Result;

	UHPUserWidget* HPPar = Cast<UHPUserWidget>(HPWidget->GetUserWidgetObject());
	if (HPPar != nullptr)
	{
		HPPar->SetVisibilityTimer();
		HPPar->HPUpdata.Broadcast(this);
	}
	if (!m_bIsBattle)
	{
		if (m_bIsSeePlayer)
		{
			m_bIsBattle = true;
		}
		else
		{
			APOPULCharacter* PC = Cast<APOPULCharacter>(DamageCauser);
			if (PC != nullptr)
			{
				AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
				if (AIController != nullptr)
					AIController->GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetKey, PC);
				m_bIsBattle = true;
				OnBattleState.Broadcast();
			}
		}
	}

	if (m_Ability.nHP <= 0)
	{
		UEnemyAnimInstance* AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance == nullptr)
		{
			this->Destroy();
			return Result;
		}
		m_bIsDeath = true;
		AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
		if (AIController == nullptr)
		{
			UE_LOG(POPUL,Warning,TEXT("AIController is nullpointer"));
			return Result;
		}
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AIController->DeleteAI();
		AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
		DefaultUIBuffer = Cast<UDefualtUserWidget>(HUD->__GetDefualtWidget());
		DefaultUIBuffer->SettingActionUI(true);
		GetWorld()->GetTimerManager().SetTimer(m_DeleteCharacterHandle, this, &AEnemyBaseCharacter::DeleteCharacter, DELETETIME);
	}
	return Result;
}

bool AEnemyBaseCharacter::GetbIsAttackRange()
{
	bool Result = Super::GetbIsAttackRange();
	USkeletalMeshComponent* Mesh = GetMesh();
	

	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	ActorsToIgnore.Add(this);
	FVector Start = Mesh->GetSocketLocation(FName(TEXT("AttackRange_Start")));
	FVector End = Mesh->GetSocketLocation(FName(TEXT("AttackRange_End")));
	int i = 0;

	bool bIsOk = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, m_AttackRadius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHits, true);
	if (bIsOk)
	{
		for (i = 0; i < OutHits.Num(); i++)
		{
			ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(OutHits.GetData()[i].GetActor());
			if (HitCharacter->GetbIsActorType() == ActorType::E_PC)
				return true;
		}
	}
	return Result;
}

void AEnemyBaseCharacter::DeleteCharacter()
{
	UE_LOG(POPUL, Warning, TEXT("캐릭터 삭제함수"));
	this->Destroy();
}

bool AEnemyBaseCharacter::Attack()
{
	GetWorld()->GetTimerManager().SetTimer(m_AttackDelayHandle, this, &AEnemyBaseCharacter::AttackDelay, m_AttackDelay);
	float fRand = rand() % 50 + 80;
	fRand = fRand * 0.01;
	m_AttackDelay = m_OriginAttckDelay * fRand;
	return true;
};

void AEnemyBaseCharacter::SetDefaultAbility(FString FName, int HP , float AttackDelay, float SKillAttackDelay , float AttackRadius , float HitDelay , int Attack , int Defend)
{
	Name = FName;
	m_Ability.nMaxHP = HP;
	m_Ability.nHP = HP;
	if (AttackDelay > 0.0f)
	{
		m_AttackDelay = AttackDelay;
		m_OriginAttckDelay = AttackDelay;
	}
	if (SKillAttackDelay > 0.0f)
		m_SkillAttackDelay = SKillAttackDelay;
	if (Attack > 0)
		m_Ability.nAttack = Attack;
	if (Defend > 0)
		m_Ability.nDefend = Defend;
	m_AttackRadius = AttackRadius;
	m_HitDelay = HitDelay;
}

void AEnemyBaseCharacter::OnSeePawn(APawn* Pawn)
{
	float SensingInterval = PawnSensing->SensingInterval;

	AEnemyAIController* EAIcontroller = Cast<AEnemyAIController>(GetController());
	USkeletalMeshComponent* Mesh = GetMesh();
	UWorld* World = GetWorld();

	if (m_Ability.nHP <= 0)
		return;

	m_bIsSeePlayer = true;
	SeePawn = Pawn;
	if (m_bIsBattle)
	{
		m_bIsBattle = true;
		OnBattleState.Broadcast();
	}
	World->GetTimerManager().SetTimer(m_SeePlayerHandle, this, &AEnemyBaseCharacter::bIsSeePlayerTimer, SensingInterval + 0.1f);
}

void AEnemyBaseCharacter::AttackHitCheck()
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
				m_bIsAttackHit = true;
			}
		}
	}
}

void AEnemyBaseCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
	
		CharacterBuffer = Cast <APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (OtherActor == CharacterBuffer)
		{
			if (CharacterBuffer->GetOverlapActor() != nullptr)
				return;
			AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
			DefaultUIBuffer = Cast<UDefualtUserWidget>(HUD->__GetDefualtWidget());
			if (bIsActorType == ActorType::E_Enemy)
			{
				if (m_Ability.nHP <= 0)
					DefaultUIBuffer->SettingActionUI(true);
			}
			else
				DefaultUIBuffer->SettingActionUI(true);

			if (DefaultUIBuffer->GetEnemyInventoryPanel() == nullptr)
				return;
			bool IsItem = false;
			int Max = DefaultUIBuffer->GetEnemyInventoryPanel()->GetChildrenCount();
			for (int i = 0; i < DefaultUIBuffer->GetEnemyInventoryPanel()->GetChildrenCount(); i++)
			{
				
				if (m_MyInventory.GetItem()[i].GetItemInformation()->nItemID != 0)
				{
					m_MyInventory.SettingInventoryItemToWidget(DefaultUIBuffer, i, bIsActorType);
					DefaultUIBuffer->SettingEnemyInvenBind(i);
					IsItem = true;
				}
			}
			CharacterBuffer->SetOverlapActor(this);
		}
	}
}

void AEnemyBaseCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this && DefaultUIBuffer != nullptr)
	{
	
		CharacterBuffer = Cast <APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		UInventorySlotUserWidget* Slot = nullptr;
		if (OtherActor == CharacterBuffer)
		{
			if (CharacterBuffer->GetOverlapActor() == this ||
				CharacterBuffer->GetOverlapActor() == nullptr)
			{
				CharacterBuffer->SetOverlapActor(nullptr);
				CharacterBuffer->SetbIsCountVisibility(false);
				CharacterBuffer = nullptr;
				DefaultUIBuffer->SettingActionUI(false);
				int Max = ENEMYBASEINVEN_MAX;
				for (int i = 0; i < Max; i++)
				{
					Slot = Cast<UInventorySlotUserWidget>(DefaultUIBuffer->GetEnemyInventoryPanel()->GetChildAt(i));
					Slot->EmptyItemData();
				}
				DefaultUIBuffer->GetEnemyInventoryBorder()->SetVisibility(ESlateVisibility::Hidden);
				DefaultUIBuffer->VisibilityDialog(ESlateVisibility::Hidden);
				AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
				UOKBoxUserWidget* OKBox = Cast<UOKBoxUserWidget>(HUD->__GetOkBoxWidget());
				OKBox->GetCountBoxBorder()->SetVisibility(ESlateVisibility::Hidden);
				DefaultUIBuffer = nullptr;
			}
		}
	}
}
