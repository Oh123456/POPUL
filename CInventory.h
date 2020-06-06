// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CItem.h"
#include "ActortypeInterface.h"

/**
 * 
 */
enum EDeleteItemenum
{
	E_ERROE	=	0,
	E_OK	=	1,
	E_OVER	=	2
};

enum EFindItem
{
	E_NOITEM = -2,
	E_SCARCE = -1
};

class POPUL_API CInventory
{
public:
	CInventory();
	~CInventory();

	void SetMaxInventoryItem(int Max);
	void Delete();

	// 인벤토리 아이템에 인벤토리 위젯을 세팅함수 Actortype에따라 적인벤토리 플레이어 인벤토리 세팅할수있다.
	void SettingInventoryItemToWidget(class UUserWidget* ItemWidget, int SettingItemNumber, int Actortype = ActorType::E_PC);
	void SettingInventoryItemToWidget_Insert(class UUserWidget* ItemWidget, int SettingItemNumber, int Actortype = ActorType::E_PC);
	// 아이템추가 HUD클레스 
	void AddItem_HUD(CItem* ItemData, class AHUD* Hud = nullptr, int Actortype = ActorType::E_PC);
	// 아이템 추가 UserWidget 클레스
	void AddItem(CItem* ItemData, class UUserWidget* ItemWidget = nullptr, int Actortype = ActorType::E_PC);
	// 아이템추가 인벤토리 위치에 아이템을 추가시킨다. Num = 인벤토리 위치
	void AddItem_Insert(CItem* ItemData, int Num,class UUserWidget* ItemWidget = nullptr, int Actortype = ActorType::E_PC);
	// 인벤토리에서 아이템코드로 아이템 찾아내기
	CItem take_Out_ItemItemCode(int ItemCode, int Count = 0);
	// 인벤토리에서 인벤토리넘버로 아이템 찾아내기
	CItem take_Out_ItemInventoyNumber(int InventoryNumber, int Count = 0);
	// 인벤토리에서 아이템코드로 아이템제거하기
	int DeleteItemfromItemCode(int ItemCode, int Count = 0);
	// 인벤토리에서 인벤토리 넘버로 아이템 제거하기
	int DeleteItemfromInventoyNumber(int InventoryNumber, int Count = 0);
	// 리턴값은 인벤토리 넘버
	int FindItem(int ItemCode);
	// 리턴값은 인벤토리 넘버
	int FindItem_CountCheck(int ItemCode, int Count);
	// 아이템 비우기
	void EmptyItem(int InventoryNumber);
	void CleanItemData();
	void SetMoney(int Vale) { m_nMoney = Vale; }
	void AddMoney(int Vale) { m_nMoney += Vale; }

	int GetMoney() const { return m_nMoney; }
	int GetMax() const { return m_New_Max; }
	bool GetbIsWidgetSet() const { return m_bIsWidgetSet; }
	CItem* GetItem() { return m_InventoryItem; }
private:
	CItem* m_InventoryItem;
	CItem m_ItemBuffer;
	int m_New_Max;
	int m_nMoney;
	bool m_bIsWidgetSet;
};
