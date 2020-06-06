// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AStruct.h"
#include "Item.generated.h"

// 사용 안함
UENUM(BlueprintType)
enum class EUItem : uint8
{
	E_UITEM		 = 0U,
	E_UINVENTORY = 1U,
	E_EQUIPMENT	 = 2U,
};
/**
 * 
 */
UCLASS()
class POPUL_API UItem : public UObject
{
	GENERATED_BODY()
	
public:
	UItem();
	void SetUItem(class CItem* cItem,int SlotNumber ,EUItem Type = EUItem::E_UINVENTORY);
	class CItem UItemToCItem();


	int GetInventorySlotNum() const { return InventorySlotNum; }
	int GetEquipmentSlotNum() const { return EquipmentSlotNum; }
private:
	class UTexture2D* ItemIcon;
	ItemStruct ItemInformation;
	FString Name;
	FString InfomationText;
	int m_IconID;
	int m_QuickSlotSettingID;


	int m_Type;
	int InventorySlotNum;
	int EquipmentSlotNum;
};
