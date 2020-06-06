// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Item.h"
#include "ItemActor.h"
#include "ItemDataSaveGame.generated.h"

// 아이템 코드 , 수량, 인벤토리슬롯위치 장비슬롯위치 구조체화해서 저장할것;



USTRUCT(BlueprintType)
struct FSaveItemdata
{
	GENERATED_BODY()

public:
	UPROPERTY()
		uint32 nItemdCode;
	UPROPERTY()
		uint32 nCount;
	UPROPERTY()
		uint32 nInventorySlotNumber;
	UPROPERTY()
		uint32 nEquipmentSlotNumber;
	UPROPERTY()
		uint32 nQuickSlotNumber;
};

enum SaveItemType
{
	E_Inventory = 1,
	E_Equipment = 2,
};
/**
 * 
 */
UCLASS()
class POPUL_API UItemDataSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UItemDataSaveGame() 
	{
		TestNaem = L"AAAAAA"; SaveSlotName = L"SaveData"; UserIndex	= 0;
	};

	FSaveItemdata CItemToFSaceItemdata(class CItem* Item, int SlotNumber,SaveItemType Typr = SaveItemType::E_Inventory);
	void SaveItem(class CItem* Item, int SlotNumber, SaveItemType Typr = SaveItemType::E_Inventory);

	UPROPERTY()
		FString SaveSlotName;
	UPROPERTY()
		FString TestNaem;
	UPROPERTY()
		uint32 UserIndex;
	UPROPERTY()
		TArray<UItem*> ItemArray;
	UPROPERTY()
		FSaveItemdata Test1;
	UPROPERTY()
		FSaveItemdata Test2[2];
	UPROPERTY()
		TArray<FSaveItemdata> SaveItemdata;
	UPROPERTY()
		FAbility PCAbility;
	UPROPERTY()
		FAbility PCOriginAbility;
	UPROPERTY()
		uint32 Money;

	//UItem Buffer;

};
