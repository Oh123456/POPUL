// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AStruct.h"

/**
 * 
 */
class POPUL_API CItem
{
public:
	CItem();
	CItem(struct FItemData Data);
	//~CItem();

	// ������ ������ ���� class CItem 
	void SetItem(CItem* Item);
	// ������ ������ ���� classUTexture2D struct UTemStruct
	void SetItem(class UTexture2D* Texture, ItemStruct* Struct);
	// ������ ������ ���� struct UTemStruct�� ����
	void SetItem(ItemStruct* Struct);
	// ������ ���� ���� �����ܸ�
	void SetItem(class UTexture2D* Texture);
	// ������ID ����
	void SetItemIcon(int Code) { m_IconID = Code; }
	// ������ID ����
	void SetQuickSlotSettingID(int ID) { m_QuickSlotSettingID = ID; }
	void SetName(FString String) { Name = String; }
	void SetItemInformation(FString String) { InfomationText = String; }
	void ZeroSet();

	int GetItemIconID() { return m_IconID; }
	int GetQuickSlotSettingID() const { return m_QuickSlotSettingID; }
	class UTexture2D* GetItemIcon() { return ItemIcon; }
	ItemStruct* GetItemInformation() { return &ItemInformation; }
	
	FString GetName() { if (!Name.IsEmpty())return Name; return L""; }
	FString GetInfomationText() { if (!InfomationText.IsEmpty()) return InfomationText; return FString(L""); }
private:
	class UTexture2D* ItemIcon;
	ItemStruct ItemInformation;
	FString Name;
	FString InfomationText;
	int m_IconID;
	int m_QuickSlotSettingID;
};
