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

	// �κ��丮 �����ۿ� �κ��丮 ������ �����Լ� Actortype������ ���κ��丮 �÷��̾� �κ��丮 �����Ҽ��ִ�.
	void SettingInventoryItemToWidget(class UUserWidget* ItemWidget, int SettingItemNumber, int Actortype = ActorType::E_PC);
	void SettingInventoryItemToWidget_Insert(class UUserWidget* ItemWidget, int SettingItemNumber, int Actortype = ActorType::E_PC);
	// �������߰� HUDŬ���� 
	void AddItem_HUD(CItem* ItemData, class AHUD* Hud = nullptr, int Actortype = ActorType::E_PC);
	// ������ �߰� UserWidget Ŭ����
	void AddItem(CItem* ItemData, class UUserWidget* ItemWidget = nullptr, int Actortype = ActorType::E_PC);
	// �������߰� �κ��丮 ��ġ�� �������� �߰���Ų��. Num = �κ��丮 ��ġ
	void AddItem_Insert(CItem* ItemData, int Num,class UUserWidget* ItemWidget = nullptr, int Actortype = ActorType::E_PC);
	// �κ��丮���� �������ڵ�� ������ ã�Ƴ���
	CItem take_Out_ItemItemCode(int ItemCode, int Count = 0);
	// �κ��丮���� �κ��丮�ѹ��� ������ ã�Ƴ���
	CItem take_Out_ItemInventoyNumber(int InventoryNumber, int Count = 0);
	// �κ��丮���� �������ڵ�� �����������ϱ�
	int DeleteItemfromItemCode(int ItemCode, int Count = 0);
	// �κ��丮���� �κ��丮 �ѹ��� ������ �����ϱ�
	int DeleteItemfromInventoyNumber(int InventoryNumber, int Count = 0);
	// ���ϰ��� �κ��丮 �ѹ�
	int FindItem(int ItemCode);
	// ���ϰ��� �κ��丮 �ѹ�
	int FindItem_CountCheck(int ItemCode, int Count);
	// ������ ����
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
