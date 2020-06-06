// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "CItem.h"

UItem::UItem()
{
	ItemInformation.ItemWidget = nullptr;
	m_QuickSlotSettingID = 0;
}

void UItem::SetUItem(CItem* cItem, int SlotNumber, EUItem Type)
{
	ItemInformation = *cItem->GetItemInformation();
	ItemIcon = cItem->GetItemIcon();
	Name = cItem->GetName();
	InfomationText = cItem->GetInfomationText();
	m_IconID = cItem->GetItemIconID();
	m_QuickSlotSettingID = cItem->GetQuickSlotSettingID();
	switch (Type)
	{
	case EUItem::E_UINVENTORY:
		InventorySlotNum = SlotNumber;
		EquipmentSlotNum = -1;
		break;
	case EUItem::E_EQUIPMENT:
		EquipmentSlotNum = SlotNumber;
		InventorySlotNum = -1;
		break;
	}
}



CItem UItem::UItemToCItem()
{
	CItem ReturnItem;
	ReturnItem.SetItem(ItemIcon, &ItemInformation);
	ReturnItem.SetItemIcon(m_IconID);
	ReturnItem.SetName(Name);
	ReturnItem.SetItemInformation(InfomationText);
	ReturnItem.SetQuickSlotSettingID(m_QuickSlotSettingID);
	return ReturnItem;
}