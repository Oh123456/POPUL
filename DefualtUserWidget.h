// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h"
#include "DefualtUserWidget.generated.h"

/**
 * 
 */

struct FEventReply;
struct FGeometry;
struct FPointerEvent;

#define CREATEITMELISTMAX 60
#define CREATEMATERIAL_MAX 6
#define DIALOGLINE_MAX 44
#define INFORMATIONTEXTBLOCK_MAX 6

UCLASS()
class POPUL_API UDefualtUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 캐릭터함수에서 바인드세팅 
	void BindCharacterSetting(class APOPULCharacter* Character);
	// 호버된 버튼 넘버 세팅
	void SetButtonHoverNumber(int value) { m_nButtonHoverNumber = value; }
	// 아이템 숫자 세팅
	void SetItemCountNumber(int Value) { m_ItemCountNumber = Value; }

	void CreateWidetOpen();
	void VisibilityDialog(ESlateVisibility Visibility);
	void VisibilityDialog(ESlateVisibility Visibility , enum DailogName Name);
	//void DialogSetting(enum DailogName Name);
	// 위젯 업데이트
	void UpDataWidget();
	//void UpDataQuickSlotWidget();
	void DialogSetting(int DialogNumber);
	void NextDialog();
	// Esc키로 UI 종료하기
	void QuitUIAndGame();

	// 퀵슬롯 세팅중인지
	void SetQuickSlotSet(bool BOOL) { m_bisQuickSlotSet = BOOL; }

	class UTextBlock* GetActionUI() { return ActionText; };
	class UBorder* GetEnemyInventoryBorder() { return EnemyInventoryBorder; }
	class UBorder* GetEnemyInventoryHeaderBorder() { return EnemyInventoryHeaderBorder; }
	class UBorder* GetInventoryBorder() { return InventoryBorder; }
	class UBorder* GetCreateBorder() { return CreateBorder; }
	class UBorder* GetInformationBorder() const { return InformationBorder; }
	class UUniformGridPanel* GetInventoryPanel() { return InventortPanel; }
	class UUniformGridPanel* GetEnemyInventoryPanel() { return EnemyInventortPanel; }
	TArray<class UInventorySlotUserWidget*>& GetEquipmentSlotWidget() { return EquipmentSlotWidget; }

	int GetHoveredNumber() const { return m_nButtonHoverNumberBuffer; };
	int GetInventoryNumber() const { return m_nInventoryNumber; }
	bool GetQuickSlotSet() const { return m_bisQuickSlotSet; }
	TArray<class UInventorySlotUserWidget*> GetQuickSlot() { return QuickSlotWidget; }
	void SettingActionUI(bool Bool);
	void SettingEnemyInvenBind(int Num);
	void SetHoverQuickSlotNum(UUserWidget* Widget);
	void SetHoverEquipmentSlot(UUserWidget* Widget);
	void SetCreateHoverNumber(int Value) { m_nCreateHoverNumber = Value; }

	virtual void Tick(FGeometry MyGeometry, float InDeltaTime);
	// 정보창 업데이트
	void UpdataInformationText();

	// UI 이동함수들
	UFUNCTION()
		struct FEventReply OnMouseButtonDown1(struct FGeometry MyGeometry, const struct FPointerEvent& MouseEvent);
	UFUNCTION()
		FEventReply OnMouseMove1(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	UFUNCTION()
		FEventReply OnMouseButtonUp1(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	UFUNCTION()
		FEventReply OnMouseMoveCreate(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	UFUNCTION()
		FEventReply OnMouseMoveEnemy(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	UFUNCTION()
		FEventReply OnMouseMoveInformation(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

protected:
	virtual void NativeConstruct() override;
	virtual void InitializeInputComponent() override;
	virtual void RegisterInputComponent();
private:
	UFUNCTION()
		void UpdataHP();
	UFUNCTION()
		void UpdataInventory();
	UFUNCTION()
		void OKboxCheckEnemy();
	UFUNCTION()
		void OKboxCheckPC();
	UFUNCTION()
		void InvenClickedFunction();
	UFUNCTION()
		void CreateItemListCheck();
	UFUNCTION()
		void CreateButtonCheck();
	UFUNCTION()
		void DialogNoButtonClick();
	UFUNCTION()
		void DialogActionButtonClick();
	UFUNCTION()
		void QuickSlotButtonClick();
	UFUNCTION()
		void EnterKey();
	UFUNCTION()
		void EquipmentClick();

private:
	void EnemyInventoryCilckFunction(class APOPULCharacter* Character , class AMainHUD* HUD);
	void PCInventoryCilckFunction(class APOPULCharacter* Character, class AMainHUD* HUD);
	bool NextDialogChcek(FString* Str,int NowNumber = 1);
private:
	UPROPERTY()
		class UBorder* BackBorder;
	UPROPERTY()
		class UProgressBar* HPProgressBar;
	UPROPERTY()
		class UTextBlock* HPText;
	UPROPERTY()
		class UTextBlock* ActionText;
	UPROPERTY()
		class UTextBlock* MoneyText;
	UPROPERTY()
		class UUniformGridPanel* InventortPanel;
	UPROPERTY()
		class UItemToolTipUserWidget* TooltipWidet;
	UPROPERTY()
		class UBorder* InventoryBorder;
	UPROPERTY()
		class UBorder* InventoryHeaderBorde;

	class UCanvasPanelSlot* BordercanvasPanelSlot;
	class UCanvasPanelSlot* CreateBordercanvasPanelSlot;
	class UCanvasPanelSlot* EnmeyBordercanvasPanelSlot;
	class UCanvasPanelSlot* InformationBordercanvasPanelSlot;


	// 적관련 UI

	UPROPERTY()
		class UUniformGridPanel* EnemyInventortPanel;
	UPROPERTY()
		class UBorder* EnemyInventoryBorder;
	UPROPERTY()
		class UBorder* EnemyInventoryHeaderBorder;

	// 제작 UI
	UPROPERTY()
		class UUniformGridPanel* CreatePanel;

	UPROPERTY()
		class UUniformGridPanel* MaterialItemPanel;
	UPROPERTY()
		class UBorder* CreateBorder;
	UPROPERTY()
		class UInventorySlotUserWidget* CreateItemListSlot[CREATEITMELISTMAX];
	UPROPERTY()
		class UInventorySlotUserWidget* InformationSlot;
	UPROPERTY()
		class UTextBlock* NameText;
	UPROPERTY()
		class UTextBlock* IteminformationText;
	UPROPERTY()
		class UTextBlock* UsingHPText;
	UPROPERTY()
		class UButton* CreateButton;

	// 대사 UI
	UPROPERTY()
		class UBorder* DialogBorder;
	UPROPERTY()
		class UButton* DialogButtonAction;
	UPROPERTY()
		class UButton* DialogButtonNo;
	UPROPERTY()
		class UTextBlock* NPCNameText;
	UPROPERTY()
		class UTextBlock* NPCDialogText;
	UPROPERTY()
		class UTextBlock* DialogActionText;
	
	//퀵슬롯 UI
	UPROPERTY()
		TArray<class UInventorySlotUserWidget*> QuickSlotWidget;

	// 정보 UI
	UPROPERTY()
		class UBorder* InformationBorder;
	UPROPERTY()
		class UBorder* InformationHeaderBorder;
	UPROPERTY()
		TArray<class UTextBlock*> InformationText;
	UPROPERTY()
		TArray<class UInventorySlotUserWidget*> EquipmentSlotWidget;

	int m_nHPBuffer;
	int m_nButtonHoverNumber;
	int m_nButtonHoverNumberBuffer;
	int m_nCreateHoverNumber;
	int m_nInventoryNumber;
	int m_ItemCountNumber;
	int m_nDialogNumber;
	bool m_isMouseButtonDown;
	bool m_bIsNextDialog;
	// 퀵슬롯 세팅중인지
	bool m_bisQuickSlotSet;
	bool m_bIsEquipmnetSlotSet;
	FVector2D fvInventoyWidgetLocation;
	FVector2D fvCreateWidgetLocation;
	FVector2D fvEnemyWidgetLocation;
	FVector2D fvInformationLocation;
	FVector2D LocationBuffer;
	FString* DialogStr;
	class UInventorySlotUserWidget* HoverSlotWidget;
	// 호버되는 퀵슬롯
	class UInventorySlotUserWidget* QuickSlotWidgetBuffer;
	// 클릭한 퀵슬롯
	class UInventorySlotUserWidget* QuickSlotWidgetClickBuffer;
	// 호버되는 장비슬롯
	class UInventorySlotUserWidget* EquipmentSlotWidgetBuffer;
	// 클릭한 장비슬롯
	class UInventorySlotUserWidget* EquipmentSlotWidgetClickBuffer;
};

typedef UDefualtUserWidget UDefaultUserWidget;