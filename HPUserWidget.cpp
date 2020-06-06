// Fill out your copyright notice in the Description page of Project Settings.

#include "HPUserWidget.h"
#include "Components/ProgressBar.h"
#include "TimerManager.h"

#include "BaseCharacter.h"

void UHPUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_ProgressBar")));
	if (HPProgressBar != nullptr)
	{
		HPProgressBar->SetVisibility(ESlateVisibility::Hidden);
		HPProgressBar->SetPercent(1.0f);
	}
	HPUpdata.AddDynamic(this, &UHPUserWidget::UpdataHP);
	//HPUpdata.AddUObject(this, &UHPUserWidget::UpdataHP);
}

void UHPUserWidget::SetVisibilityTimer()
{
	if (HPProgressBar->GetVisibility() == ESlateVisibility::Hidden)
		HPProgressBar->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(VisibilityTimerHandle, this, &UHPUserWidget::SetVisibility, VISIBILITYTIME);
}

void UHPUserWidget::SetVisibility()
{
	HPProgressBar->SetVisibility(ESlateVisibility::Hidden);
}

void UHPUserWidget::SetVisibility(ESlateVisibility Visibility)
{
	HPProgressBar->SetVisibility(Visibility);
}

void UHPUserWidget::UpdataHP(ABaseCharacter* Character)
{
	if (Character == nullptr)
	{
		HPProgressBar->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	Ability* CharacterAbility = Character->GetAbility();
	float fMaxHP = (float)CharacterAbility->nMaxHP;
	float fHP = (float)CharacterAbility->nHP;
	float Percent = fHP / fMaxHP;

	if (fHP <= 0)
	{
		HPProgressBar->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	HPProgressBar->SetPercent(Percent);
}