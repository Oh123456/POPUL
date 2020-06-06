// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemDataSaveGame.h"

#include "CItem.h"

FSaveItemdata UItemDataSaveGame::CItemToFSaceItemdata(CItem* Item, int SlotNumber, SaveItemType Typr)
{
	FSaveItemdata ReturnData;
	ReturnData.nItemdCode = Item->GetItemInformation()->nItemID;
	ReturnData.nCount = Item->GetItemInformation()->nCount;
	ReturnData.nQuickSlotNumber = Item->GetQuickSlotSettingID();
	switch (Typr)
	{
	case SaveItemType::E_Inventory:
		ReturnData.nEquipmentSlotNumber = -1;
		ReturnData.nInventorySlotNumber = SlotNumber;
		break;
	case SaveItemType::E_Equipment:
		ReturnData.nInventorySlotNumber = -1;
		ReturnData.nEquipmentSlotNumber = SlotNumber;
		break;
	}
	Test1 = ReturnData;
	Test2[0] = ReturnData;
	return ReturnData;

}


void UItemDataSaveGame::SaveItem(class CItem* Item, int SlotNumber, SaveItemType Typr)
{
	SaveItemdata.Add(UItemDataSaveGame::CItemToFSaceItemdata(Item, SlotNumber, Typr));
	
}
