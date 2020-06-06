// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageFontActor.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

#include "DamageTextUserWidget.h"

// Sets default values
ADamageFontActor::ADamageFontActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageText = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageText"));
	DamageText->SetupAttachment(RootComponent);
	DamageText->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> DAMAGE_TEXT(TEXT("/Game/Widget/DamageWidgetBlueprint.DamageWidgetBlueprint_C"));
	if (DAMAGE_TEXT.Succeeded())
	{
		DamageText->SetWidgetClass(DAMAGE_TEXT.Class);
		DamageText->SetDrawSize(FVector2D(200.0f, 25.0f));
	}
}

// Called when the game starts or when spawned
void ADamageFontActor::BeginPlay()
{
	Super::BeginPlay();
	ADamageFontActor::VisibilityTimer();
}

// Called every frame
void ADamageFontActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z += 0.225f;
	SetActorLocation(ActorLocation);
}

void ADamageFontActor::SetDamageText(int Value)
{
	if (DamageText != nullptr)
	{
		UDamageTextUserWidget* DamageFont = Cast<UDamageTextUserWidget>(DamageText->GetUserWidgetObject());
		if (DamageFont != nullptr)
		{
			DamageFont->SetDamageText(Value);
		}
	}
}

void ADamageFontActor::VisibilityTimer(float Time)
{
	GetWorld()->GetTimerManager().SetTimer(DamageTextTimerHanlde, this, &ADamageFontActor::VisibilityTimerFunction, Time);
}

void ADamageFontActor::VisibilityTimerFunction()
{
	this->Destroy();
}