// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CItem.h"

/**
 * 
 */

enum EquipmentItemKinds
{
	E_Weapone	= 0,
	E_Armor		= 1,
};



class POPUL_API CEquipment
{
public:
	CEquipment();
	~CEquipment();
	
	void SetWidget(class UUserWidget* ItemSlotWidget);
	CItem* GetEquipmentItem(EquipmentItemKinds Kinds) { if (EquipmentItem.Num() > Kinds) return &EquipmentItem.GetData()[Kinds]; return nullptr; }

	TArray<CItem>* GetEquipmentItem() { return &EquipmentItem; }
private:
	TArray<CItem> EquipmentItem;
};
