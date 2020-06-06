// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemToolTipUserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"

#include "InventorySlotUserWidget.h"
#include "CItem.h"

#include <wchar.h>

void UItemToolTipUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	NameTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameTextBlock")));
	ItemTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemTextBlock")));
	ItemAbilityTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemAbilityTextBlock")));
	GoldTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("GoldTextBlock")));

}

void UItemToolTipUserWidget::SetItemInformaition(UInventorySlotUserWidget* ItemSlotWidget)
{

	CItem* ItemData = ItemSlotWidget->GetItemData();
	ItemStruct* ItemStructData = ItemData->GetItemInformation();
	Ability* ItemAbility = &ItemStructData->ItemAbility;
	FString String(L"");
	FString AbilityString(L"");
	if (ItemData == nullptr)
		return;
	
	if (!NameTextBlock->IsValidLowLevel())
		return;
	NameTextBlock->SetText(UKismetTextLibrary::Conv_StringToText(ItemData->GetName()));

	AbilityString = IntItemAbilityInformaition(ItemAbility->nMaxHP, L"최대 체력" , AbilityString);
	
	if (ItemAbility->nHP != 0)
	{
		String = UKismetStringLibrary::Concat_StrStr(L"체 력", L" : ");
		String = UKismetStringLibrary::Concat_StrStr(String, UKismetStringLibrary::Conv_IntToString(ItemAbility->nHP));
		if (ItemAbility->nHP > 0)
			String = UKismetStringLibrary::Concat_StrStr(String, L" 회복 \n");
		else
			String = UKismetStringLibrary::Concat_StrStr(String, L" 감소 \n");
		AbilityString = UKismetStringLibrary::Concat_StrStr(AbilityString, String);
	}

	AbilityString = IntItemAbilityInformaition(ItemAbility->nAttack, L"공격력", AbilityString);
	AbilityString = IntItemAbilityInformaition(ItemAbility->nDefend, L"방어력", AbilityString);
	AbilityString = IntItemAbilityInformaition(ItemAbility->nSTR, L"힘", AbilityString);
	AbilityString = IntItemAbilityInformaition(ItemAbility->nDEX, L"민 첩", AbilityString);
	AbilityString = floatItemAbilityInformaition(ItemAbility->nAttackSpeed, L"공격 속도", AbilityString);
	AbilityString = IntItemAbilityInformaition(ItemAbility->nMoveSpeed, L"이동 속도", AbilityString);

	ItemAbilityTextBlock->SetText(UKismetTextLibrary::Conv_StringToText(AbilityString));
	
	FString InfomaitionString = UItemToolTipUserWidget::Newline(ItemData->GetInfomationText());
	ItemTextBlock->SetText(UKismetTextLibrary::Conv_StringToText(InfomaitionString));
	GoldTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(ItemStructData->nGold));
}

FString UItemToolTipUserWidget::IntItemAbilityInformaition(int Data, FString AbilityName, FString StackString)
{
	FString String;
	if (Data != 0)
	{
		String = UKismetStringLibrary::Concat_StrStr(AbilityName, L" : ");
		String = UKismetStringLibrary::Concat_StrStr(String, UKismetStringLibrary::Conv_IntToString(Data));
		if (Data > 0)
			String = UKismetStringLibrary::Concat_StrStr(String, L" 증가 \n");
		else
			String = UKismetStringLibrary::Concat_StrStr(String, L" 감소 \n");

		return UKismetStringLibrary::Concat_StrStr(StackString, String);
	}

	return UKismetStringLibrary::Concat_StrStr(StackString, L"");
}

FString UItemToolTipUserWidget::floatItemAbilityInformaition(float Data, FString AbilityName, FString StackString)
{
	FString String;
	if (Data != 0)
	{
		String = UKismetStringLibrary::Concat_StrStr(AbilityName, L" : ");
		String = UKismetStringLibrary::Concat_StrStr(String, UKismetStringLibrary::Conv_FloatToString(Data));
		String = UKismetStringLibrary::Concat_StrStr(String, L" %");

		if (Data > 0)
			String = UKismetStringLibrary::Concat_StrStr(String, L" 증가 \n");
		else
			String = UKismetStringLibrary::Concat_StrStr(String, L" 감소 \n");

		return UKismetStringLibrary::Concat_StrStr(StackString, String);
	}

	return UKismetStringLibrary::Concat_StrStr(StackString, L"");
}

void UItemToolTipUserWidget::SetNameTextBlock(FText Text)
{
	NameTextBlock->SetText(Text);
}

void UItemToolTipUserWidget::SetNameTextBlock(FString String)
{
	UItemToolTipUserWidget::SetNameTextBlock(UKismetTextLibrary::Conv_StringToText(String));
}

void UItemToolTipUserWidget::SetItemTextBlock(FText Text)
{
	ItemTextBlock->SetText(Text);
}

void UItemToolTipUserWidget::SetItemTextBlock(FString String)
{
	UItemToolTipUserWidget::SetItemTextBlock(UKismetTextLibrary::Conv_StringToText(String));
}



#define LINE_MAX 16
FString UItemToolTipUserWidget::Newline(FString String)
{
	TArray<TCHAR> StringtoChar = String.GetCharArray();
	FString ReturnString;
	FString StringBuffer;
	TCHAR CharBuffer[LINE_MAX * 2];
	int nMax = StringtoChar.Max();
	int i = 0;
	int CharNumber = 0;
	float fLineMax = LINE_MAX;
	float fCount = 0.0f;
	memset(CharBuffer, 0, sizeof(CharBuffer));
	if (StringtoChar.GetData() == nullptr)
		return String;
	for (i = 0; i < nMax; i++)
	{
		CharBuffer[CharNumber] = StringtoChar.GetData()[i];
		CharNumber++;
		if (wcscmp(&StringtoChar.GetData()[i], L" ") == 0)
			fCount += 0.5f;
		else
			fCount += 1.0f;

		if (fCount >= fLineMax  || i >= (nMax - 1))
		{
			
			StringBuffer = UKismetStringLibrary::Concat_StrStr(CharBuffer,"\n");
			ReturnString = UKismetStringLibrary::Concat_StrStr(ReturnString, StringBuffer);
			memset(CharBuffer, 0, sizeof(wchar_t) * LINE_MAX * 2);
			CharNumber = 0;
			fCount = 0.0f;
		}
	}
	return ReturnString;
}

#define CHAR_MAX 100
FString UItemToolTipUserWidget::Newline(FString String, int Max)
{
	TArray<TCHAR> StringtoChar = String.GetCharArray();
	FString ReturnString;
	FString StringBuffer;
	TCHAR CharBuffer[CHAR_MAX];
	int nMax = StringtoChar.Max();
	int i = 0;
	int CharNumber = 0;
	float fLineMax = Max;
	float fCount = 0.0f;
	memset(CharBuffer, 0, sizeof(CharBuffer));
	if (StringtoChar.GetData() == nullptr)
		return String;
	for (i = 0; i < nMax; i++)
	{
		CharBuffer[CharNumber] = StringtoChar.GetData()[i];
		CharNumber++;
		if (wcscmp(&StringtoChar.GetData()[i], L" ") == 0)
			fCount += 0.5f;
		else
			fCount += 1.0f;

		if (fCount >= fLineMax || i >= (nMax - 1))
		{

			StringBuffer = UKismetStringLibrary::Concat_StrStr(CharBuffer, "\n");
			ReturnString = UKismetStringLibrary::Concat_StrStr(ReturnString, StringBuffer);
			memset(CharBuffer, 0, sizeof(wchar_t) * Max);
			CharNumber = 0;
			fCount = 0.0f;
		}
	}
	return ReturnString;
}