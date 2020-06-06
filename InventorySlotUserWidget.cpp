// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlotUserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Widgets/SToolTip.h"

#include "MainHUD.h"
#include "CItem.h"
#include "ItemToolTipUserWidget.h"
#include "TextureManager.h"
#include "DefualtUserWidget.h"


void UInventorySlotUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//UTexture2D* Text;
	m_nItemID = 0;
	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("InvenButton")));
	CountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CountTextBlock")));
	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("IconImage")));
	if (ItemImage != nullptr)
	{
		AMainHUD* hud = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetHUD());
		//ItemImage->SetBrushFromTexture(hud->TTTT);
		// 블루 프린트로 옮김 
		ItemButton->OnHovered.AddDynamic(this, &UInventorySlotUserWidget::OnHoverToolTip);
		ItemButton->OnUnhovered.AddDynamic(this, &UInventorySlotUserWidget::OnUnHover);
	}
	m_SlotNumber = -1;
	m_nInventotyNumber = -1;
	QuickSlotID = 0;
	IsHover = nullptr;
	IsQuickSlot = false;
	IsHoverQulckSlot = false;
	//SetRenderScale(FVector2D(1.0f, 1.0f));
}

// 블루프린트로 옮길것
void UInventorySlotUserWidget::OnHoverToolTip()
{
	if (this == nullptr)
		return;

	AMainHUD* hud = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (hud == nullptr)
		return;
	UDefualtUserWidget* Default = Cast<UDefualtUserWidget>(hud->__GetDefualtWidget());
	if (Default == nullptr)
		return;
	//Tooltip->SetItemInformaition(this);
	Default->SetButtonHoverNumber(m_SlotNumber);
	Default->SetCreateHoverNumber(m_nInventotyNumber);
	Default->SetHoverQuickSlotNum(this);
	Default->SetHoverEquipmentSlot(this);
	//UUserWidget* ToolTipUserWidget = hud->__GetToolTipWidget();
	// 블루 프린트로 옮김
	UItemToolTipUserWidget* Tooltip = hud->__GetToolTipWidget();
	Tooltip->SetItemInformaition(this);
	//if (Tooltip != nullptr)
	//{
	//	//Tooltip = Cast<UItemToolTipUserWidget>(ToolTipUserWidget);
	//	if (Tooltip == nullptr)
	//		return;
	//	Tooltip->SetItemInformaition(this);
	//	//if (m_ItemData != nullptr)
	//	//	if (IsValid(ItemButton))
	//	//	{
	//	//		ItemButton->SetToolTip(Tooltip);
	//	//	}
	//	if (Tooltip->IsValidLowLevel())
	//	{
	//		{
	//			if (m_ItemData != nullptr)
	//				if (IsValid(ItemButton))
	//					ItemButton->SetToolTip(Tooltip);
	//		}
	//	}
	//}
	//Tooltip->SetItemInformaition(this);
	////if (this->m_ItemData == nullptr)
	////	return;

	//if (IsValid(Tooltip))
	//{
	//	if (Tooltip->IsValidLowLevel())
	//	{
	//		if (m_ItemData != nullptr)
	//			ItemButton->SetToolTip(Tooltip);
	//	}
	//}
	//// 왜 툴팁에서 에러가 날까.. 
	//if (&IsQuickSlot != 0x0000000000000000)
	//{
	//	if (IsQuickSlot)
	//	{
	//		if (&IsHoverQulckSlot != 0x0000000000000000)
	//			IsHoverQulckSlot = true;
	//		//IsHoverQulckSlot = true;
	//		//IsHover = this;
	//	}
	//}
}

void UInventorySlotUserWidget::OnUnHover()
{
	// 에러해결
	IsHoverQulckSlot = false;
	AMainHUD* hud = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UDefualtUserWidget* Default = Cast<UDefualtUserWidget>(hud->__GetDefualtWidget());
	Default->SetHoverQuickSlotNum(nullptr);
	//IsHover = nullptr;
}

void UInventorySlotUserWidget::SetSlotItemData_Dummy(class CItem ItemData, int Count)
{

	m_DummyItemData.SetItem(&ItemData);
	m_DummyItemData.SetItemIcon(m_DummyItemData.GetItemIconID());
	m_DummyItemData.SetItem(ATextureManager::GetTextrues()[m_DummyItemData.GetItemIconID()]);
	UInventorySlotUserWidget::SetSlotItemData(&m_DummyItemData);
	if (Count == 0)
		CountText->SetText(UKismetTextLibrary::Conv_StringToText(L""));
	else
		CountText->SetText(UKismetTextLibrary::Conv_IntToText(Count));
}

void UInventorySlotUserWidget::SetSlotItemData(CItem* ItemData)
{ 
	
	if (this == nullptr && ItemData == nullptr)
		return;
	m_ItemData = ItemData;
	if (m_ItemData->GetItemInformation() == nullptr)
		return;
	ItemImage->SetBrushFromTexture(m_ItemData->GetItemIcon());
	m_nItemID = m_ItemData->GetItemInformation()->nItemID;
	if (m_ItemData->GetItemInformation()->nCount != 0)
		CountText->SetText(UKismetTextLibrary::Conv_IntToText(m_ItemData->GetItemInformation()->nCount));
}


void UInventorySlotUserWidget::EmptyItemData()
{
	if (this == nullptr)
		return;
	if (m_ItemData != nullptr)
	{
		m_ItemData->SetQuickSlotSettingID(0);
		m_ItemData->GetItemInformation()->ItemWidget = nullptr;
		m_ItemData = nullptr;
	}
	ItemButton->SetToolTip(nullptr);
	ItemImage->SetBrushFromTexture(nullptr);
	m_nItemID = 0;
	CountText->SetText(FText::FromString(L""));
}

void UInventorySlotUserWidget::UpdataWidget()
{
	
	if (m_ItemData == nullptr || this == nullptr)
		return;
	ItemImage->SetBrushFromTexture(m_ItemData->GetItemIcon());
	m_nItemID = m_ItemData->GetItemInformation()->nItemID;
	if (m_ItemData->GetItemInformation()->nCount != 0)
		CountText->SetText(UKismetTextLibrary::Conv_IntToText(m_ItemData->GetItemInformation()->nCount));
}