// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDataInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"

#include "POPULCharacter.h"
#include "NameTypes.h"
#include "StorageCharacter.h"
#include "ItemDataSaveGame.h"


UDataTable* UGameDataInstance::ItemData;
UDataTable* UGameDataInstance::DialogData;
bool UGameDataInstance::bIsMoveLevel = false;

UGameDataInstance::UGameDataInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Data(TEXT("/Game/Data/ItemData.ItemData"));
	if (Data.Succeeded())
	{
		ItemData = Data.Object;

	}
	static ConstructorHelpers::FObjectFinder<UDataTable> ShopDialog(TEXT("/Game/Data/DailogShop.DailogShop"));
	if (ShopDialog.Succeeded())
	{
		DialogData = ShopDialog.Object;
	}
	PCIsDeath = false;
	LoadData = false;
	m_bIsPCBattel = true;
	UGameDataInstance::LoadGameDataCheack();
}

FItemData* UGameDataInstance::GetItemData(int ItemCode)
{
	//bIsMoveLevel = false;
	FName ItemName = UKismetStringLibrary::Conv_StringToName(UKismetStringLibrary::Conv_IntToString(ItemCode));
	return ItemData->FindRow<FItemData>(ItemName, TEXT(""));
}

void UGameDataInstance::SavePCDataOpenLevel(class APOPULCharacter* PC)
{
	//memcpy(&PCAbility, PC->GetAbility(), sizeof(FAbility));
	//memcpy(&PCInventory, PC->GetInventory(), sizeof(CInventory));
	CInventory* PlayerInventory = PC->GetInventory();
	PCAbility = *PC->GetAbility();
	PCOriginAbility = *PC->GetOriginAbility();
	PCInventory.SetMaxInventoryItem(PlayerInventory->GetMax());
	PCInventory.SetMoney(PlayerInventory->GetMoney());
	m_bIsPCBattel = PC->GetIsBattel();
	int Max = PlayerInventory->GetMax();
	int i, j;
	for (i = 0; i < Max; i++)
	{
		if (PlayerInventory->GetItem()[i].GetItemInformation()->nItemID != 0)
		{
			PCInventory.AddItem_Insert(&PlayerInventory->GetItem()[i], i);
			PCInventory.GetItem()[i].SetQuickSlotSettingID(PlayerInventory->GetItem()[i].GetQuickSlotSettingID());
		}

	}

	PCEquipment.GetEquipmentItem()->GetData()[EquipmentItemKinds::E_Weapone] = *PC->GetEquipmet()->GetEquipmentItem(EquipmentItemKinds::E_Weapone);
	PCEquipment.GetEquipmentItem()->GetData()[EquipmentItemKinds::E_Armor] = *PC->GetEquipmet()->GetEquipmentItem(EquipmentItemKinds::E_Armor);
	PCEquipment.GetEquipmentItem(EquipmentItemKinds::E_Weapone)->GetItemInformation()->ItemWidget = nullptr;
	PCEquipment.GetEquipmentItem(EquipmentItemKinds::E_Armor)->GetItemInformation()->ItemWidget = nullptr;
	PCIsDeath = PC->GetbIsDeath();

	TArray<AActor*> arryStorageCharacter;
	UGameplayStatics::GetAllActorsOfClass((UObject*)GetWorld(), AStorageCharacter::StaticClass(), arryStorageCharacter);
	int StorageMax = arryStorageCharacter.Num();
	if (StorageMax == 0)
		return;
	StorageInventory = new CInventory[StorageMax];

	for (i = 0; i < StorageMax; i++)
	{
		AStorageCharacter* Storage = Cast<AStorageCharacter>(arryStorageCharacter.GetData()[i]);
		if (Storage == nullptr)
			continue;
		int ItemMax = Storage->GetInventory()->GetMax();
		StorageInventory[i].SetMaxInventoryItem(ItemMax);
		for (j = 0; j < ItemMax; j++)
		{
			if (Storage->GetInventory()->GetItem()[j].GetItemInformation()->nItemID != 0)
				StorageInventory[i].AddItem(&Storage->GetInventory()->GetItem()[j]);
		}
	}
	
}

void UGameDataInstance::LoadPCDataOpenLevel(class APOPULCharacter* PC)
{
	// 에러있느지 확인ㄴ
	PC->LoadAbility(PCAbility, PCOriginAbility);
	PC->LoadInventory(PCInventory);
	PC->LoadEquipment(PCEquipment);
	PC->SetbIsDeath(PCIsDeath);
	PC->SetWeaponVisibility(m_bIsPCBattel);
	if (PC->GetAbility()->nHP <= 0)
		PC->GetAbility()->nHP = PC->GetAbility()->nMaxHP;
	PC->OnHPChange.Broadcast();

	TArray<AActor*> arryStorageCharacter;
	UGameplayStatics::GetAllActorsOfClass((UObject*)GetWorld(), AStorageCharacter::StaticClass(), arryStorageCharacter);
	int StorageMax = arryStorageCharacter.Num();
	if (StorageMax == 0)
		return;
	for (int i = 0; i < StorageMax; i++)
	{
		AStorageCharacter* Storage = Cast<AStorageCharacter>(arryStorageCharacter.GetData()[i]);
		if (Storage == nullptr)
			continue;
		int ItemMax = Storage->GetInventory()->GetMax();
		for (int j = 0; j < ItemMax; j++)
		{

			if (StorageInventory != nullptr)
			{
				if (StorageInventory[i].GetItem() != nullptr)
				{
					if (StorageInventory[i].GetItem()[j].GetItemInformation()->nItemID != 0)
						Storage->GetInventory()->AddItem(&StorageInventory[i].GetItem()[j]);
				}
			}
		}
	}
	if (StorageInventory != nullptr)
		delete[] StorageInventory;
}

void UGameDataInstance::LoadGameDataCheack()
{
	SaveData = Cast<UItemDataSaveGame>(UGameplayStatics::CreateSaveGameObject(UItemDataSaveGame::StaticClass()));
	SaveData = Cast<UItemDataSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveData->SaveSlotName, SaveData->UserIndex));
	if (SaveData == nullptr)
		LoadData = false;
	else
		LoadData = true;
}

void UGameDataInstance::LoadGameData()
{
	APOPULCharacter* PC = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter((UObject*)GetWorld(), 0));
	SaveData = Cast<UItemDataSaveGame>(UGameplayStatics::CreateSaveGameObject(UItemDataSaveGame::StaticClass()));
	SaveData = Cast<UItemDataSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveData->SaveSlotName, SaveData->UserIndex));
	PCInventory.SetMaxInventoryItem(81);
	int i = 0;
	CInventory* PCInven = PC->GetInventory();
	if (SaveData != nullptr)
	{
		PC->bIsMoveLevel = true;
		for (i = 0; i < SaveData->SaveItemdata.Num(); i++)
		{
			//UItem* uItem = SaveData->ItemArray.GetData()[i];
			FSaveItemdata* SaveItemdata = &SaveData->SaveItemdata.GetData()[i];
			CItem cItem (*GetItemData(SaveItemdata->nItemdCode));
			if (SaveItemdata->nInventorySlotNumber != -1)
			{
				int SlotNumber = SaveItemdata->nInventorySlotNumber;
				cItem.GetItemInformation()->nCount = SaveItemdata->nCount;
				PCInventory.AddItem_Insert(&cItem, SlotNumber);
				PCInventory.GetItem()[SlotNumber].SetQuickSlotSettingID(SaveItemdata->nQuickSlotNumber);
			}
			else if (SaveItemdata->nEquipmentSlotNumber != -1)
			{
				CItem EItem(*GetItemData(SaveItemdata->nItemdCode));
				EItem.GetItemInformation()->nCount = SaveItemdata->nCount;
				switch (SaveItemdata->nEquipmentSlotNumber)
				{
				case EquipmentItemKinds::E_Weapone:
					PCEquipment.GetEquipmentItem()->GetData()[EquipmentItemKinds::E_Weapone] = EItem;
					PCEquipment.GetEquipmentItem(EquipmentItemKinds::E_Weapone)->GetItemInformation()->ItemWidget = nullptr;
					break;
				case EquipmentItemKinds::E_Armor:
					PCEquipment.GetEquipmentItem()->GetData()[EquipmentItemKinds::E_Armor] = EItem;
					PCEquipment.GetEquipmentItem(EquipmentItemKinds::E_Armor)->GetItemInformation()->ItemWidget = nullptr;
					break;
				}
			}

		}
		PCAbility = SaveData->PCAbility;
		PCOriginAbility = SaveData->PCOriginAbility;
		PCInventory.SetMoney(SaveData->Money);

		//LoadData = false;
	}
}