// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CItem.h"
#include "InventorySlotUserWidget.generated.h"

class CItem;
/**
 * 
 */

//enum EquipmentSlotNumber
//{
//	E_Weapone = 0,
//	E_Armor = 1,
//};


UCLASS()
class POPUL_API UInventorySlotUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	class UImage* GetItemImage() { return ItemImage; }
	class CItem* GetItemData() { return m_ItemData; };

	void SetSlotItemID(int Value) { m_nItemID = Value; }
	void SetSlotItemData(class CItem* ItemData);
	void SetSlotItemData_Dummy(class CItem ItemData, int Count = 0);
	void SetInventoryNumber(int Value) { m_nInventotyNumber = Value; }
	void SetWidgetSlotNumber(int Value) { m_SlotNumber = Value; }
	void SetQuickSlotID(int ID) { QuickSlotID = ID; }
	void SetIsQuickSlot(bool BOOL) { IsQuickSlot = BOOL; }
	void SetEquipmentSlotNumber(int Value) { m_EquipmentSlotNumber = Value; }

	class UButton* GetItemButton() { return ItemButton; }
	int GetSlotItemID() { return m_nItemID; }
	int GetInventoryNumber() const { return m_nInventotyNumber; }
	int GetQuickSlotID() const { return QuickSlotID; }
	int GetEquipmentSlotNumber() const { return m_EquipmentSlotNumber; }
	bool GetIsHoverQulckSlot() const { return IsHoverQulckSlot; }
	// 마우스 위에있는 슬롯포인터
	UUserWidget* GetHoverWidgetData() { return IsHover; }
	// m_Itemdata is nullptr Return True ,Not nullptr Return False
	UFUNCTION(BlueprintCallable, Category = "InventorySlotUserWidget", meta = (DisplayName = "IsItemData"))
		bool IsItemData() { if (m_ItemData == nullptr) return true; return false; }
	UFUNCTION(BlueprintCallable, Category = "InventorySlotUserWidget")
		void OnHoverToolTip();
	UFUNCTION()
		void OnUnHover();

	// 위젯 초기화
	void UpdataWidget();
	// 위젯에서 아이템 데이터 초기화
	void EmptyItemData();
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY()
		class UButton* ItemButton;
	UPROPERTY()
		class UImage* ItemImage;
	UPROPERTY()
		class UTextBlock* CountText;
	UUserWidget* IsHover;
	// 퀵슬롯인지
	bool IsQuickSlot;
	// 퀵슬롯을 호버했는지
	bool IsHoverQulckSlot;
	// 퀵슬롯 아이디
	int QuickSlotID;
	// 아이템 ID
	int m_nItemID;
	// 인벤토리 넘버
	int m_nInventotyNumber;
	// 슬롯넘버
	int m_SlotNumber;
	// 장비 슬롯 넘버
	int m_EquipmentSlotNumber;
	// 아이템데이터
	class CItem* m_ItemData;
	//아이템 더미데이터
	CItem m_DummyItemData;
};
