// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageTextUserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Styling/SlateColor.h"


void UDamageTextUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Damage_Text")));
	FSlateColor Color(FLinearColor::Red);
	DamageText->SetColorAndOpacity(Color);
	//DamageText->SetText(UKismetTextLibrary::Conv_IntToText(Damage));
	//DamageTextDelegate.AddDynamic(this , &UDamageTextUserWidget::SetDamageText);
}

void UDamageTextUserWidget::SetVisibility_Visible()
{
	DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Damage_Text")));
	//if (DamageText != nullptr)
		DamageText->SetVisibility(ESlateVisibility::Visible);
}

void UDamageTextUserWidget::SetVisibility_Hidden()
{
	if (DamageText != nullptr)
		DamageText->SetVisibility(ESlateVisibility::Hidden);
}

void UDamageTextUserWidget::SetDamageText(int Damage)
{
	// 왜 널포인트?? 
	DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Damage_Text")));
	if (DamageText != nullptr)
		DamageText->SetText(UKismetTextLibrary::Conv_IntToText(Damage));
}

