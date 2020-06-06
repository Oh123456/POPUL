// Fill out your copyright notice in the Description page of Project Settings.

#include "CInventory.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

#include "GameDataInstance.h"
#include "DefualtUserWidget.h"
#include "InventorySlotUserWidget.h"
#include "TextureManager.h"
#include "MainHUD.h"



CInventory::CInventory()
{
	m_InventoryItem = nullptr;
	m_nMoney = 0;
	m_New_Max = 0;
}

CInventory::~CInventory()
{
	//if (m_InventoryItem != nullptr)
	//{
	//	if (m_New_Max != 0)
	//	{
	//		if (m_New_Max == 1)
	//		{
	//			delete m_InventoryItem;
	//			m_InventoryItem = nullptr;
	//		}
	//		else
	//		{
	//			delete[] m_InventoryItem;
	//			m_InventoryItem = nullptr;
	//		}

	//	}
	//}
}

void CInventory::Delete()
{
	if (m_InventoryItem != nullptr)
	{
		if (m_New_Max != 0)
		{
			if (m_New_Max == 1)
			{
				delete m_InventoryItem;
				m_InventoryItem = nullptr;
				m_New_Max = 0;
			}
			else
			{
				delete[] m_InventoryItem;
				m_InventoryItem = nullptr;
				m_New_Max = 0;
			}

		}
	}
}

void CInventory::SetMaxInventoryItem(int Max)
{
	m_InventoryItem = new CItem[Max];

	m_New_Max = Max;
	for (int i = 0; i < Max; i++)
		memset(&m_InventoryItem[i], 0, sizeof(CItem));

}

void CInventory::AddItem_HUD(CItem* ItemData, class AHUD* Hud, int Actortype)
{
	UUserWidget* Widget = nullptr;

	if (Hud == nullptr)
	{
		CInventory::AddItem(ItemData, Widget, Actortype);
		return;
	}
	AMainHUD* MainHUD = Cast<AMainHUD>(Hud);
	if (MainHUD == nullptr)
	{
		CInventory::AddItem(ItemData, Widget, Actortype);
		return;
	}
	CInventory::AddItem(ItemData, MainHUD->__GetDefualtWidget(), Actortype);
	

}

void CInventory::AddItem(CItem* ItemData, UUserWidget* ItemWidget, int Actortype)
{
	if (m_New_Max <= 1)
		return;
	UDefaultUserWidget* DefaultWidget = Cast<UDefualtUserWidget>(ItemWidget);
	int i = 0;
	for (i = 0; i < m_New_Max; i++)
	{
		if (m_InventoryItem != nullptr)
		{
			if (m_InventoryItem[i].GetItemInformation()->nItemID == ItemData->GetItemInformation()->nItemID)
			{
				switch (m_InventoryItem[i].GetItemInformation()->Itemtype)
				{
				case Itemtype::E_EQUIPMENT_WEAPONE:
				case Itemtype::E_EQUIPMENT_ARMOR:
					if (m_InventoryItem[i].GetItemInformation()->nCount == 1)
						continue;
				default:
					if (m_InventoryItem[i].GetItemInformation()->nCount == 999)
						continue;
					break;
				}
				m_InventoryItem[i].GetItemInformation()->nCount += ItemData->GetItemInformation()->nCount;
				if (m_InventoryItem[i].GetItemInformation()->nCount > 999)
				{
					ItemData->GetItemInformation()->nCount = m_InventoryItem[i].GetItemInformation()->nCount - 999;
					m_InventoryItem[i].GetItemInformation()->nCount = 999;
					if (m_bIsWidgetSet != false)
					{
						if (DefaultWidget != nullptr)
							DefaultWidget->UpDataWidget();
					}
						//m_InventoryItem[i].GetItemInformation()->ItemWidget->UpdataWidget();
					break;
				}
				else
				{
					if (m_bIsWidgetSet != false)
					{
						if (DefaultWidget != nullptr)
							DefaultWidget->UpDataWidget();
					}
						//m_InventoryItem[i].GetItemInformation()->ItemWidget->UpdataWidget();
					return;
				}
			}
		}
	}
	bool bIsOver = false;
	for (i = 0; i < m_New_Max; i++)
	{
		if (m_InventoryItem != nullptr)
		{
			if (m_InventoryItem[i].GetItemInformation()->nItemID == 0)
			{
				int Countbuffer = 0;
				switch (ItemData->GetItemInformation()->Itemtype)
				{
				case Itemtype::E_EQUIPMENT_WEAPONE:
				case Itemtype::E_EQUIPMENT_ARMOR:
					if (ItemData->GetItemInformation()->nCount > 1)
					{
						Countbuffer = ItemData->GetItemInformation()->nCount - 1;
						ItemData->GetItemInformation()->nCount = 1;
						bIsOver = true;
					}
					else
						bIsOver = false;
					break;
				default:
					if (ItemData->GetItemInformation()->nCount > 999)
					{
						Countbuffer = ItemData->GetItemInformation()->nCount - 999;
						ItemData->GetItemInformation()->nCount = 999;
						bIsOver = true;
					}
					else
						bIsOver = false;
					break;
				}
				m_InventoryItem[i].SetItem(ItemData);
				m_InventoryItem[i].GetItemInformation()->InventoryNumber = i;
				m_InventoryItem[i].SetItem(ATextureManager::GetTextrues()[ItemData->GetItemIconID()]);
				//m_InventoryItem[i].SetItem(ItemData->GetItemIcon() ,ItemData->GetItemInformation());

				CInventory::SettingInventoryItemToWidget(ItemWidget, i, Actortype);
				if (bIsOver)
				{
					ItemData->GetItemInformation()->nCount = Countbuffer;
					CInventory::AddItem(ItemData, ItemWidget, Actortype);
				}
				if (DefaultWidget != nullptr)
					DefaultWidget->UpDataWidget();
				break;
			}
		}
	}
}

void CInventory::AddItem_Insert(CItem* ItemData, int Num, class UUserWidget* ItemWidget  ,int Actortype )
{
	if (m_New_Max <= 1)
		return;
	UDefaultUserWidget* DefaultWidget = Cast<UDefualtUserWidget>(ItemWidget);
	bool bIsOver = false;

	//for (i = 0; i < m_New_Max; i++)
	{
		if (m_InventoryItem != nullptr)
		{
			if (m_InventoryItem[Num].GetItemInformation()->nItemID == 0)
			{
				int Countbuffer = 0;
				if (ItemData->GetItemInformation()->nCount > 999)
				{
					Countbuffer = ItemData->GetItemInformation()->nCount - 999;
					ItemData->GetItemInformation()->nCount = 999;
					bIsOver = true;
				}
				else
					bIsOver = false;
				m_InventoryItem[Num].SetItem(ItemData);
				m_InventoryItem[Num].GetItemInformation()->InventoryNumber = Num;
				m_InventoryItem[Num].SetItem(ATextureManager::GetTextrues()[ItemData->GetItemIconID()]);
				//m_InventoryItem[i].SetItem(ItemData->GetItemIcon() ,ItemData->GetItemInformation());

				CInventory::SettingInventoryItemToWidget_Insert(ItemWidget, Num, Actortype);
				if (bIsOver)
				{
					ItemData->GetItemInformation()->nCount = Countbuffer;
					CInventory::AddItem(ItemData, ItemWidget, Actortype);
				}
				if (DefaultWidget != nullptr)
					DefaultWidget->UpDataWidget();
			}
		}
	}
}

void CInventory::SettingInventoryItemToWidget(class UUserWidget* ItemWidget, int SettingItemNumber, int Actortype)
{
	if (ItemWidget == nullptr)
	{
		m_bIsWidgetSet = false;
		return;
	}
	m_bIsWidgetSet = true;
	UDefualtUserWidget* DefaultWidget = Cast<UDefualtUserWidget>(ItemWidget);
	UUniformGridPanel* InventoryPanel = nullptr; //DefaultWidget->GetInventoryPanel();
	UInventorySlotUserWidget* InventorySlotWidget = nullptr;
	int i = 0;
	int Max = 0;

	switch (Actortype)
	{
	case ActorType::E_PC:
		InventoryPanel = DefaultWidget->GetInventoryPanel();
		break;
	case ActorType::E_NPC:
	case ActorType::E_Enemy:
	case ActorType::E_BOX:
		InventoryPanel = DefaultWidget->GetEnemyInventoryPanel();
		break;
	default:
		break;
	}

	if (InventoryPanel == nullptr)
	{
		m_bIsWidgetSet = false;
		return;
	}

	Max = InventoryPanel->GetChildrenCount();


	for (i = 0; i < Max; i++)
	{
		if (InventoryPanel == nullptr)
			break;
		if (InventorySlotWidget == nullptr)
		{
			InventorySlotWidget = Cast<UInventorySlotUserWidget>(InventoryPanel->GetChildAt(i));
			if (InventorySlotWidget->GetItemData() != nullptr)
			{
				InventorySlotWidget = nullptr;
				continue;
			}
			//InventorySlotWidget->SetSlotItemID(m_InventoryItem[SettingItemNumber].GetItemInformation()->nItemID);
			InventorySlotWidget->SetSlotItemData(&m_InventoryItem[SettingItemNumber]);
			InventorySlotWidget->SetInventoryNumber(SettingItemNumber);
			InventorySlotWidget->SetWidgetSlotNumber(i);
			//InventorySlotWidget->GetItemImage()->SetBrushFromTexture(m_InventoryItem[SettingItemNumber].GetItemIcon());
			m_InventoryItem[SettingItemNumber].GetItemInformation()->ItemWidget = InventorySlotWidget;
			break;
		}
	}

}

void CInventory::SettingInventoryItemToWidget_Insert(class UUserWidget* ItemWidget, int SettingItemNumber, int Actortype )
{
	if (ItemWidget == nullptr)
	{
		m_bIsWidgetSet = false;
		return;
	}
	m_bIsWidgetSet = true;
	UDefualtUserWidget* DefaultWidget = Cast<UDefualtUserWidget>(ItemWidget);
	UUniformGridPanel* InventoryPanel = nullptr; //DefaultWidget->GetInventoryPanel();
	UInventorySlotUserWidget* InventorySlotWidget = nullptr;
	int Max = 0;

	switch (Actortype)
	{
	case ActorType::E_PC:
		InventoryPanel = DefaultWidget->GetInventoryPanel();
		break;
	case ActorType::E_NPC:
	case ActorType::E_Enemy:
	case ActorType::E_BOX:
		InventoryPanel = DefaultWidget->GetEnemyInventoryPanel();
		break;
	default:
		break;
	}

	if (InventoryPanel == nullptr)
	{
		m_bIsWidgetSet = false;
		return;
	}

	Max = InventoryPanel->GetChildrenCount();


	
	{
		if (InventoryPanel == nullptr)
			return;
		if (InventorySlotWidget == nullptr)
		{
			InventorySlotWidget = Cast<UInventorySlotUserWidget>(InventoryPanel->GetChildAt(SettingItemNumber));
			if (InventorySlotWidget->GetItemData() != nullptr)
			{
				InventorySlotWidget = nullptr;
				return;
			}
			//InventorySlotWidget->SetSlotItemID(m_InventoryItem[SettingItemNumber].GetItemInformation()->nItemID);
			InventorySlotWidget->SetSlotItemData(&m_InventoryItem[SettingItemNumber]);
			InventorySlotWidget->SetInventoryNumber(SettingItemNumber);
			InventorySlotWidget->SetWidgetSlotNumber(SettingItemNumber);
			//InventorySlotWidget->GetItemImage()->SetBrushFromTexture(m_InventoryItem[SettingItemNumber].GetItemIcon());
			m_InventoryItem[SettingItemNumber].GetItemInformation()->ItemWidget = InventorySlotWidget;
		}
	}
}

CItem CInventory::take_Out_ItemItemCode(int ItemCode, int Count)
{
	int Returnvalue = CInventory::DeleteItemfromItemCode(ItemCode, Count);
	if (Returnvalue == EDeleteItemenum::E_ERROE)
		return m_ItemBuffer;
	if (Returnvalue == EDeleteItemenum::E_OK)
		return m_ItemBuffer;
	else if (Returnvalue == EDeleteItemenum::E_OVER)
	{
		// 갯수초과시 만들어라
		return m_ItemBuffer;
	}
	return m_ItemBuffer;
}

int CInventory::DeleteItemfromItemCode(int ItemCode, int Count)
{
	int ret = EDeleteItemenum::E_ERROE;
	if (m_New_Max <= 1)
		return ret;
	int i = 0;

	for (i = 0; i < m_New_Max; i++)
	{
		if (m_InventoryItem != nullptr)
		{
			if (m_InventoryItem[i].GetItemInformation()->nItemID == ItemCode)
			{
				ret = EDeleteItemenum::E_OK;
				memcpy(&m_ItemBuffer, &m_InventoryItem[i],sizeof(CItem));
				//m_ItemBuffer = m_InventoryItem[i];
				m_InventoryItem[i].GetItemInformation()->nCount -= Count;
				if (m_InventoryItem[i].GetItemInformation()->nCount <= 0)
				{
					UInventorySlotUserWidget* SlotWidget = m_InventoryItem[i].GetItemInformation()->ItemWidget;
					if (SlotWidget != nullptr)
						if (m_bIsWidgetSet != false)
							SlotWidget->EmptyItemData();
					memset(&m_InventoryItem[i], 0, sizeof(CItem));
					//EmptyItem(i);
					ret = EDeleteItemenum::E_OVER;
				}
				break;
			}
		}
	}

	return ret;
}

CItem CInventory::take_Out_ItemInventoyNumber(int InventoryNumber, int Count)
{
	int Returnvalue = CInventory::DeleteItemfromItemCode(InventoryNumber, Count);
	if (Returnvalue == EDeleteItemenum::E_ERROE)
		return m_ItemBuffer;
	if (Returnvalue == EDeleteItemenum::E_OK)
		return m_ItemBuffer;
	else if (Returnvalue == EDeleteItemenum::E_OVER)
	{
		// 갯수초과시 만들어라
		return m_ItemBuffer;
	}
	return m_ItemBuffer;
}

int CInventory::DeleteItemfromInventoyNumber(int InventoryNumber, int Count)
{
	int ret = EDeleteItemenum::E_ERROE;
	if (&m_InventoryItem[InventoryNumber] == nullptr
		|| InventoryNumber >= m_New_Max)
		return ret;

	//m_ItemBuffer = m_InventoryItem[InventoryNumber];
	memcpy(&m_ItemBuffer, &m_InventoryItem[InventoryNumber], sizeof(CItem));
	m_InventoryItem[InventoryNumber].GetItemInformation()->nCount -= Count;
	ret = EDeleteItemenum::E_OK;
	if (m_InventoryItem[InventoryNumber].GetItemInformation()->nCount <= 0)
	{
		UInventorySlotUserWidget* SlotWidget = m_InventoryItem[InventoryNumber].GetItemInformation()->ItemWidget;
		if (SlotWidget != nullptr)
			if (m_bIsWidgetSet != false)
				SlotWidget->EmptyItemData();
		memset(&m_InventoryItem[InventoryNumber], 0, sizeof(CItem));
		//CInventory::EmptyItem(InventoryNumber);
		ret = EDeleteItemenum::E_OVER;
	}

	return ret;
}


void CInventory::EmptyItem(int InventoryNumber)
{
	if (m_bIsWidgetSet == false)
		return;
	UInventorySlotUserWidget* SlotWidget = m_InventoryItem[InventoryNumber].GetItemInformation()->ItemWidget;
	if (SlotWidget != nullptr)
		SlotWidget->EmptyItemData();
	memset(&m_InventoryItem[InventoryNumber], 0, sizeof(CItem));

	//m_InventoryItem[InventoryNumber].GetInfomationText() = L"";
	//m_InventoryItem[InventoryNumber].GetName() = L"";
	//m_InventoryItem[InventoryNumber].GetItemIcon = nullptr;
	//memset(m_InventoryItem[InventoryNumber].GetItemInformation(), 0, sizeof(FItemStruct));
}

void CInventory::CleanItemData()
{
	for (int i = 0; i < m_New_Max; i++)
	{
		if (m_InventoryItem[i].GetItemInformation() != nullptr)
		{
			if (m_InventoryItem[i].GetItemInformation()->nCount <= 0)
				CInventory::EmptyItem(i);
		}
	}
	

}

int CInventory::FindItem(int ItemCode)
{
	for (int i = 0; i < m_New_Max; i++)
	{
		if (m_InventoryItem == nullptr)
			break;
		if (m_InventoryItem[i].GetItemInformation()->nItemID == ItemCode)
			return i;
		
	}
	return -1;
}

int CInventory::FindItem_CountCheck(int ItemCode, int Count)
{
	for (int i = 0; i < m_New_Max; i++)
	{
		if (m_InventoryItem == nullptr)
			break;
		if (m_InventoryItem[i].GetItemInformation()->nItemID == ItemCode)
		{
			if (m_InventoryItem[i].GetItemInformation()->nCount >= Count)
				return i;
			else
				return EFindItem::E_SCARCE;
		}
	}
	return EFindItem::E_NOITEM;
}