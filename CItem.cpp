// Fill out your copyright notice in the Description page of Project Settings.

#include "CItem.h"
#include "GameDataInstance.h"
#include "TextureManager.h"
#include "Engine/Texture2D.h"

CItem::CItem()
{
	ItemInformation.ItemWidget = nullptr;
	m_QuickSlotSettingID = 0;
}

CItem::CItem(FItemData Data)
{
	Name									= Data.fsName;
	InfomationText							= Data.fsItemText;
	ItemInformation.Itemtype				= Data.nItemtype;
	m_IconID								= Data.nImageCode;
	ItemInformation.nItemID					= Data.nItemID;
	ItemInformation.ItemAbility.nAttack		= Data.nAttack;
	ItemInformation.ItemAbility.nDefend		= Data.nDefend;
	ItemInformation.ItemAbility.nAttackSpeed= Data.nAttackSpeed;
	ItemInformation.ItemAbility.nMoveSpeed  = Data.nMoveSpeed;
	ItemInformation.ItemAbility.nSTR		= Data.nSTR;
	ItemInformation.ItemAbility.nDEX		= Data.nDEX;
	ItemInformation.ItemAbility.nHP			= Data.nHP;
	ItemInformation.ItemAbility.nMaxHP		= Data.nMaxHP;
	ItemInformation.nGold					= Data.nGold;


	ItemInformation.Create.bIsCreate		 = Data.bIsCreate;
	ItemInformation.Create.nmaterialCount1	 = Data.nmaterialCount1;
	ItemInformation.Create.nmaterialCount2	 = Data.nmaterialCount2;
	ItemInformation.Create.nmaterialCount3	 = Data.nmaterialCount3;
	ItemInformation.Create.nmaterialCount4   = Data.nmaterialCount4;
	ItemInformation.Create.nmaterialCount5	 = Data.nmaterialCount5;
	ItemInformation.Create.nmaterialCount6	 = Data.nmaterialCount6;
	ItemInformation.Create.nmaterialItemID1  = Data.nmaterialItemID1;
	ItemInformation.Create.nmaterialItemID2  = Data.nmaterialItemID2;
	ItemInformation.Create.nmaterialItemID3  = Data.nmaterialItemID3;
	ItemInformation.Create.nmaterialItemID4  = Data.nmaterialItemID4;
	ItemInformation.Create.nmaterialItemID5  = Data.nmaterialItemID5;
	ItemInformation.Create.nmaterialItemID6  = Data.nmaterialItemID6;
	ItemInformation.Create.nCreateUsingHP	 = Data.nCreateUsingHP;
	ItemInformation.Create.nReturnCount		 = Data.nReturnCount;
	m_QuickSlotSettingID = 0;
}

//CItem::~CItem()
//{
//
//}


void CItem::SetItem(UTexture2D* Texture, ItemStruct* Struct)
{
	//CItem::SetItem(Texture);
	//CItem::SetItem(Struct);
	ItemIcon = Texture;
	ItemInformation = *Struct;
	//memset(&ItemInformation, 0, sizeof(ItemStruct));
	//memcpy(&ItemInformation, Struct, sizeof(ItemStruct));
}

void CItem::SetItem(ItemStruct* Struct)
{
	memset(&ItemInformation, 0, sizeof(ItemStruct));
	memcpy(&ItemInformation, Struct, sizeof(ItemStruct));
}

void CItem::SetItem(UTexture2D* Texture)
{
	ItemIcon = Texture;
	//memset(ItemIcon, 0 ,sizeof(UTexture2D));
	//memcpy(ItemIcon, Texture, sizeof(UTexture2D));
}

void CItem::SetItem(CItem* Item)
{
	if (Item == nullptr)
		return;
	if (Item->GetItemIcon() == nullptr)
		Item->SetItem(ATextureManager::GetTextrues()[Item->GetItemIconID()]);
	this->SetItem(Item->GetItemIcon(), Item->GetItemInformation());
	Name = Item->GetName();
	InfomationText = Item->GetInfomationText();
	m_IconID = Item->GetItemIconID();
}

void CItem::ZeroSet()
{
	Name									= 0;
	InfomationText							= 0;
	ItemInformation.Itemtype				= 0;
	m_IconID								= 0;
	ItemInformation.nItemID					= 0;
	ItemInformation.ItemAbility.nAttack		= 0;
	ItemInformation.ItemAbility.nDefend		= 0;
	ItemInformation.ItemAbility.nAttackSpeed= 0;
	ItemInformation.ItemAbility.nMoveSpeed  = 0;
	ItemInformation.ItemAbility.nSTR		= 0;
	ItemInformation.ItemAbility.nDEX		= 0;
	ItemInformation.ItemAbility.nHP			= 0;
	ItemInformation.ItemAbility.nMaxHP		= 0;
	ItemInformation.nGold					= 0;


	ItemInformation.Create.bIsCreate		 = false;
	ItemInformation.Create.nmaterialCount1	 = 0;
	ItemInformation.Create.nmaterialCount2	 = 0;
	ItemInformation.Create.nmaterialCount3	 = 0;
	ItemInformation.Create.nmaterialCount4   = 0;
	ItemInformation.Create.nmaterialCount5	 = 0;
	ItemInformation.Create.nmaterialCount6	 = 0;
	ItemInformation.Create.nmaterialItemID1  = 0;
	ItemInformation.Create.nmaterialItemID2  = 0;
	ItemInformation.Create.nmaterialItemID3  = 0;
	ItemInformation.Create.nmaterialItemID4  = 0;
	ItemInformation.Create.nmaterialItemID5  = 0;
	ItemInformation.Create.nmaterialItemID6  = 0;
	ItemInformation.Create.nCreateUsingHP	 = 0;
	ItemInformation.Create.nReturnCount		 = 0;
	m_QuickSlotSettingID = 0;
}