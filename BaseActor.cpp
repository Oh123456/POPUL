// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Engine.h"
#include "POPULGameMode.h"
#include "POPULCharacter.h"
#include "MainHUD.h"
#include "CItem.h"
#include "CInventory.h"
#include "DefualtUserWidget.h"


// Sets default values
ABaseActor::ABaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefualtSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefualtSceneComponent"));
	RootComponent = DefualtSceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->AttachTo(DefualtSceneComponent);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseActor::BeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseActor::EndOverlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	BoxComponent->AttachTo(StaticMeshComponent);

	
}



// Called when the game starts or when spawned
void ABaseActor::BeginPlay()
{
	m_MyInventory.SetMaxInventoryItem(DEFUALT_ACTOR_INVENTORYMAX);
	// 물리실행
	StaticMeshComponent->SetSimulatePhysics(true);
	Super::BeginPlay();

	BoxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	bIsActorType = ActorType::E_DestructionObject;
	m_bIsOverlap = false;
}


// Called every frame
void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABaseActor::Destroy(bool bNetForce , bool bShouldModifyLevel )
{
	bool pendingkillpednding = Super::Destroy();
	if (!pendingkillpednding)
		m_MyInventory.Delete();
	return pendingkillpednding;
}


void ABaseActor::SetItem(EItemName ItemCode, int ItemCount)
{
	APOPULGameMode* GameMode = (APOPULGameMode*)UGameplayStatics::GetGameMode((UObject*)GetWorld());
	Items = GameMode->GetItemData((int32)ItemCode);
	Items.GetItemInformation()->nCount = ItemCount;
	ABaseActor::m_MyInventory.AddItem(&Items, nullptr);
}


void ABaseActor::SetInventoryMaxSetting(int Max)
{
	m_MyInventory.SetMaxInventoryItem(Max);
}

bool ABaseActor::PercentageFunction(float ItemPercentage, int Percentage)
{
	float fRand = (float)(rand() % (Percentage * 100)) + 1.0f;
	float fPercentage = fRand / Percentage;
	if (fPercentage <= ItemPercentage)
		return true;
	return false;

}


void ABaseActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	if (OtherActor != this)
	{
		APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (OtherActor == Character)
		{
			AMainHUD* Hud = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
			UDefualtUserWidget* UI = Cast<UDefualtUserWidget>(Hud->__GetDefualtWidget());
			UI->SettingActionUI(true);
			Character->SetOverlapActor(this);
		}
		ABaseActor* BaseActor = Cast<ABaseActor>(OtherActor);
		if (BaseActor != nullptr)
			m_bIsOverlap = true;
		
	}
}

void ABaseActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
	
		APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		APOPULGameMode* GameMode = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (OtherActor == Character)
		{
			if (Character->GetOverlapActor() != this)
				return;
			AMainHUD* Hud = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
			UDefualtUserWidget* UI = Cast<UDefualtUserWidget>(Hud->__GetDefualtWidget());
			UI->SettingActionUI(false);
			Character->SetOverlapActor(nullptr);
			if (bIsActorType == ActorType::E_CreateObject)
			{
				UI->GetCreateBorder()->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}