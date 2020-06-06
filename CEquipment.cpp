// Fill out your copyright notice in the Description page of Project Settings.

#include "CEquipment.h"

#include "GameDataInstance.h"

CEquipment::CEquipment()
{
	CItem Weaponedummy;
	CItem Armordummy;
	Weaponedummy.ZeroSet();
	Weaponedummy.GetItemInformation()->Itemtype = Itemtype::E_EQUIPMENT_WEAPONE;
	Weaponedummy.GetItemInformation()->nItemID = 0;
	Armordummy.ZeroSet();
	Armordummy.GetItemInformation()->Itemtype = Itemtype::E_EQUIPMENT_ARMOR;
	Armordummy.GetItemInformation()->nItemID = 0;

	EquipmentItem.Add(Weaponedummy);
	EquipmentItem.Add(Armordummy);
}

CEquipment::~CEquipment()
{
}

void CEquipment::SetWidget(class UUserWidget* ItemSlotWidget)
{

}

