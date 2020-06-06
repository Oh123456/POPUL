// Fill out your copyright notice in the Description page of Project Settings.

#include "DefualtUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetLayoutLibrary.h"
#include "Geometry.h"
#include "Input/Events.h"

#include "POPULCharacter.h"
#include "POPULGameMode.h"
#include "EnemyBaseCharacter.h"
#include "InventorySlotUserWidget.h"
#include "ItemToolTipUserWidget.h"
#include "OKboxUserWidget.h"
#include "MainHUD.h"
#include "TextureManager.h"
#include "CInventory.h"
#include "GameDataInstance.h"
#include "Engine.h"

void UDefualtUserWidget::BindCharacterSetting(class APOPULCharacter* Character)
{
	Character->OnHPChange.AddUObject(this,&UDefualtUserWidget::UpdataHP);
	m_nHPBuffer = 0;
	Character->OnMoneyChange.AddUObject(this, &UDefualtUserWidget::UpdataInventory);
}

void  UDefualtUserWidget::SettingEnemyInvenBind(int Num)
{

}

void UDefualtUserWidget::RegisterInputComponent()
{
	Super::RegisterInputComponent();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("입력됨"));
	InputComponent->BindAction("EnterKey", IE_Pressed, this, &UDefualtUserWidget::EnterKey);
}

void UDefualtUserWidget::EnterKey()
{
	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	APlayerController* PCController = Cast<APlayerController>(Character->GetController());
	AMainHUD* MainHUD = Cast<AMainHUD>(PCController->GetHUD());
	UOKBoxUserWidget* OKBox = Cast<UOKBoxUserWidget>(MainHUD->__GetOkBoxWidget());
	UDefualtUserWidget* DefaultUserWidget = Cast<UDefualtUserWidget>(MainHUD->__GetDefualtWidget());
	if (OKBox->GetOKBoxBorder()->GetVisibility() == ESlateVisibility::Visible)
		OKBox->YesButtonClickedFunction();
	if (OKBox->GetCountBoxBorder()->GetVisibility() == ESlateVisibility::Visible)
		OKBox->CountYesButtonClickedFunction();
}

void UDefualtUserWidget::InitializeInputComponent()
{
	Super::InitializeInputComponent();
	InputComponent->BindAction("EnterKey", IE_Pressed, this, &UDefualtUserWidget::EnterKey);
}

void UDefualtUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_Bar")));
	AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UGameDataInstance* Data = Cast<UGameDataInstance>(GetGameInstance());

	if (HPProgressBar != nullptr)
	{
		HPProgressBar->SetPercent(1.0f);
	}
	HPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPText")));
	if (HPText != nullptr)
	{
		
		if (Character != nullptr)
		{
			
			FString Test(L"123456789");

			HPText->SetText(UKismetTextLibrary::Conv_StringToText(Test));
			// 콤마가 적용됨 ex 123,456,789
			//HPText->SetText(UKismetTextLibrary::Conv_IntToText(Character->nHP));
			Character->OnHPChange.Broadcast();
		}
	}

	InventortPanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("InventoryUnifomGridPanel")));
	if (InventortPanel != nullptr)
	{

		Character->GetInventory()->SetMaxInventoryItem(InventortPanel->GetChildrenCount());
		Character->InvenMax = InventortPanel->GetChildrenCount();
		bool bIsWidgetSet = Character->GetInventory()->GetbIsWidgetSet();
		AMainHUD* hud = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
		UInventorySlotUserWidget* Buffer = nullptr;// = Cast<UInventorySlotUserWidget>(InventortPanel->GetChildAt(0));
		UUniformGridSlot* InventortSlot = nullptr;//
		int R = 0 , C = 0;
		for (int i = 0; i < InventortPanel->GetChildrenCount(); i++)
		{
			if (InventortPanel->GetChildAt(i) == nullptr)
				continue;
			Buffer = Cast<UInventorySlotUserWidget>(InventortPanel->GetChildAt(i));
			InventortSlot = UWidgetLayoutLibrary::SlotAsUniformGridSlot(Buffer);
			InventortSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			InventortSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			InventortSlot->SetRow(R);
			InventortSlot->SetColumn(C);
			if (Buffer->GetItemButton() != nullptr)
				Buffer->GetItemButton()->OnClicked.AddDynamic(this, &UDefualtUserWidget::OKboxCheckPC);
			C++;
			if (C >= 9)
			{
				R++;
				C = 0;
			}
		}


		SetToolTip(GetWidgetFromName(TEXT("/Game/Widget/Inven/NewWidgetBlueprint.NewWidgetBlueprint")));
	}

	// 인벤토리 UI
	InventoryBorder = Cast<UBorder>(GetWidgetFromName(TEXT("InventoryBorder")));
	if (InventoryBorder != nullptr)
	{
		InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
		BordercanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InventoryBorder);
		BordercanvasPanelSlot->SetPosition(FVector2D(0.0f,0.0f));
		fvInventoyWidgetLocation = FVector2D(0.0f, 0.0f);
		InventoryBorder->OnMouseButtonUpEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonUp1);
		
	}

	InventoryHeaderBorde = Cast<UBorder>(GetWidgetFromName(TEXT("InventoryHeaderBorde")));
	if (InventoryHeaderBorde != nullptr)
	{
		InventoryHeaderBorde->OnMouseButtonDownEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonDown1);
		InventoryHeaderBorde->OnMouseMoveEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseMove1);
		InventoryHeaderBorde->OnMouseButtonUpEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonUp1);
	}


	EnemyInventortPanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("EnemyInventoryUnifomGridPanel")));
	if (EnemyInventortPanel != nullptr)
	{
		UInventorySlotUserWidget* Buffer = nullptr;
		UUniformGridSlot* InventortSlot = nullptr;
		int R = 0, C = 0;
		for (int i = 0; i < ENEMYITEMSLOTMAX; i++)
		{
			Buffer = Cast<UInventorySlotUserWidget>(HUD->__GetEnemyIemSlotWidget()[i]);
			InventortSlot = EnemyInventortPanel->AddChildToUniformGrid(HUD->__GetEnemyIemSlotWidget()[i]);
			InventortSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			InventortSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			InventortSlot->SetRow(R);
			InventortSlot->SetColumn(C);
			if (Buffer->GetItemButton() != nullptr)
				Buffer->GetItemButton()->OnClicked.AddDynamic(this, &UDefualtUserWidget::OKboxCheckEnemy);
			
			C++;
			if (C >= 9)
			{
				R++;
				C = 0;
			}
		}


	}

	EnemyInventoryBorder = Cast<UBorder>(GetWidgetFromName(TEXT("EnemyInvenBorder")));
	if (EnemyInventoryBorder != nullptr)
	{
		
		EnemyInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
		EnmeyBordercanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(EnemyInventoryBorder);
		EnmeyBordercanvasPanelSlot->SetPosition(FVector2D(0.0f, 0.0f));
		fvInventoyWidgetLocation = FVector2D(0.0f, 0.0f);
		InventoryBorder->OnMouseButtonUpEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonUp1);

	}
	EnemyInventoryHeaderBorder = Cast<UBorder>(GetWidgetFromName(TEXT("EnemyInvenHeaderBorder")));
	if (EnemyInventoryHeaderBorder)
	{
		EnemyInventoryHeaderBorder->OnMouseButtonDownEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonDown1);
		EnemyInventoryHeaderBorder->OnMouseMoveEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseMoveEnemy);
		EnemyInventoryHeaderBorder->OnMouseButtonUpEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonUp1);
	}

	
	CreateBorder = Cast<UBorder>(GetWidgetFromName(TEXT("CreateBorder")));
	if (CreateBorder != nullptr)
	{
		CreateBordercanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CreateBorder);
		CreateBordercanvasPanelSlot->SetPosition(FVector2D(0.0f, 0.0f));
		fvCreateWidgetLocation = FVector2D(0.0f, 0.0f);
		CreateBorder->OnMouseButtonDownEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonDown1);
		CreateBorder->OnMouseMoveEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseMoveCreate);
		CreateBorder->OnMouseButtonUpEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonUp1);
	}

	CreatePanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("CreateItemListUniformGridPanel")));
	if (CreatePanel != nullptr)
	{
		UInventorySlotUserWidget* Buffer = nullptr;
		UUniformGridSlot* InventortSlot = nullptr;
		int R = 0, C = 0;

		
		for (int i = 0; i < CREATEITMELISTMAX; i++)
		{
			CreateItemListSlot[i] = Cast<UInventorySlotUserWidget>(HUD->__GetIemSlotWidget()[i]);
			InventortSlot = CreatePanel->AddChildToUniformGrid(HUD->__GetIemSlotWidget()[i]);
			InventortSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			InventortSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			InventortSlot->SetRow(R);
			InventortSlot->SetColumn(C);
			if (CreateItemListSlot[i]->GetItemButton() != nullptr)
				CreateItemListSlot[i]->GetItemButton()->OnClicked.AddDynamic(this, &UDefualtUserWidget::CreateItemListCheck);
			C++;
			if (C >= 9)
			{
				R++;
				C = 0;
			}
			if (i >= CREATEITMELISTMAX)
			{
				InventortSlot->SetRow(7);
				InventortSlot->SetColumn(6);
				CreateItemListSlot[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}

	}
	CreateBorder->SetVisibility(ESlateVisibility::Hidden);
	UsingHPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("UsingHPTextBlock")));
	UsingHPText->SetText(FText::FromString(L"0"));

	UBorder* BackBorder = Cast<UBorder>(GetWidgetFromName(TEXT("BackBorder")));
	if (BackBorder != nullptr)
		BackBorder->OnMouseButtonUpEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonUp1);
	NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameTextBlock")));
	IteminformationText = Cast<UTextBlock>(GetWidgetFromName(TEXT("InformationTextBlock")));

	FString Infomation(L"            ");
	Infomation = UItemToolTipUserWidget::Newline(Infomation, 9);
	NameText->SetText(UKismetTextLibrary::Conv_StringToText(L"  "));
	IteminformationText->SetText(UKismetTextLibrary::Conv_StringToText(Infomation));

	CreateButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateButtonOk")));
	if (CreateButton != nullptr)
		CreateButton->OnClicked.AddDynamic(this, &UDefualtUserWidget::CreateButtonCheck);
	
	InformationSlot = Cast<UInventorySlotUserWidget>(GetWidgetFromName(TEXT("InforSlot")));
	MaterialItemPanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("MaterialItemUniformGridPanel")));
	ActionText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ActionTextBlock")));
	ActionText->SetVisibility(ESlateVisibility::Hidden);
	MoneyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MoneyTextBlock")));
	Character->OnMoneyChange.Broadcast();

	// 대사부분 
	DialogButtonNo = Cast<UButton>(GetWidgetFromName(TEXT("DialohNoButton")));
	if (DialogButtonNo != nullptr)
		DialogButtonNo->OnClicked.AddDynamic(this, &UDefualtUserWidget::DialogNoButtonClick);
	DialogButtonAction = Cast<UButton>(GetWidgetFromName(TEXT("DialohActionButton")));
	if (DialogButtonAction != nullptr)
		DialogButtonAction->OnClicked.AddDynamic(this, &UDefualtUserWidget::DialogActionButtonClick);
	DialogBorder = Cast<UBorder>(GetWidgetFromName(TEXT("DialogBorder")));
	if (DialogBorder != nullptr)
		DialogBorder->SetVisibility(ESlateVisibility::Hidden);
	NPCNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NPCNameTextBlock")));
	NPCDialogText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DialogTextBlock")));
	DialogActionText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DialogActionTextBlocks")));

	UDefualtUserWidget::VisibilityDialog(ESlateVisibility::Hidden);
	// 퀵슬롯
	UInventorySlotUserWidget* SlotWidget1 = Cast<UInventorySlotUserWidget>(GetWidgetFromName(TEXT("QuickSlot1")));
	if (SlotWidget1 != nullptr)
	{
		SlotWidget1->GetItemButton()->OnClicked.AddDynamic(this, &UDefualtUserWidget::QuickSlotButtonClick);
		SlotWidget1->SetQuickSlotID(1);
		SlotWidget1->SetIsQuickSlot(true);
	}
	UInventorySlotUserWidget* SlotWidget2 = Cast<UInventorySlotUserWidget>(GetWidgetFromName(TEXT("QuickSlot2")));
	if (SlotWidget2 != nullptr)
	{
		SlotWidget2->GetItemButton()->OnClicked.AddDynamic(this, &UDefualtUserWidget::QuickSlotButtonClick);
		SlotWidget2->SetQuickSlotID(2);
		SlotWidget2->SetIsQuickSlot(true);
	}
	UInventorySlotUserWidget* SlotWidget3 = Cast<UInventorySlotUserWidget>(GetWidgetFromName(TEXT("QuickSlot3")));
	if (SlotWidget3 != nullptr)
	{
		SlotWidget3->GetItemButton()->OnClicked.AddDynamic(this, &UDefualtUserWidget::QuickSlotButtonClick);
		SlotWidget3->SetQuickSlotID(3);
		SlotWidget3->SetIsQuickSlot(true);
	}
	UInventorySlotUserWidget* SlotWidget4 = Cast<UInventorySlotUserWidget>(GetWidgetFromName(TEXT("QuickSlot4")));
	if (SlotWidget4 != nullptr)
	{
		SlotWidget4->GetItemButton()->OnClicked.AddDynamic(this, &UDefualtUserWidget::QuickSlotButtonClick);
		SlotWidget4->SetQuickSlotID(4);
		SlotWidget4->SetIsQuickSlot(true);
	}
	QuickSlotWidget.Add(SlotWidget1);
	QuickSlotWidget.Add(SlotWidget2);
	QuickSlotWidget.Add(SlotWidget3);
	QuickSlotWidget.Add(SlotWidget4);
	

	// Ok박스
	UOKBoxUserWidget* OKBox = Cast<UOKBoxUserWidget>(HUD->__GetOkBoxWidget());
	OKBox->OnOkBoxInven.BindDynamic(this, &UDefualtUserWidget::InvenClickedFunction);
	UDefualtUserWidget::CreateWidetOpen();
	m_isMouseButtonDown = false;
	m_bisQuickSlotSet = false;
	m_nButtonHoverNumber = 0;
	m_nButtonHoverNumberBuffer = 0;
	m_nCreateHoverNumber = -1;
	m_nDialogNumber = 0;
	QuickSlotWidgetBuffer = nullptr;
	//정보 UI
	m_bIsEquipmnetSlotSet = false;
	InformationBorder = Cast<UBorder>(GetWidgetFromName(TEXT("InformationBorder")));
	if (InformationBorder != nullptr)
	{
		InformationBorder->SetVisibility(ESlateVisibility::Hidden);
		InformationBordercanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InformationBorder);
		fvInformationLocation = FVector2D(0.0f, 0.0f);
		InformationBorder->OnMouseButtonUpEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonUp1);

	}
	InformationHeaderBorder = Cast<UBorder>(GetWidgetFromName(TEXT("InformationBorderHeader")));
	if (InformationHeaderBorder != nullptr)
	{
		InformationHeaderBorder->OnMouseButtonDownEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonDown1);
		InformationHeaderBorder->OnMouseMoveEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseMoveInformation);
		InformationHeaderBorder->OnMouseButtonUpEvent.BindDynamic(this, &UDefualtUserWidget::OnMouseButtonUp1);
	}

	UInventorySlotUserWidget* EquipmentSlot[2]
	{
		Cast<UInventorySlotUserWidget>(GetWidgetFromName(TEXT("Weapon_Equipment_InventorySlotWidgetBlueprint"))),
		Cast<UInventorySlotUserWidget>(GetWidgetFromName(TEXT("Armor_Equipment_InventorySlotWidgetBlueprint"))),
	};

	for (int i = 0; i < 2; i++)
	{
		// 바인딩할것
		EquipmentSlot[i]->SetEquipmentSlotNumber(i);
		EquipmentSlot[i]->GetItemButton()->OnClicked.AddDynamic(this, &UDefualtUserWidget::EquipmentClick);
		EquipmentSlotWidget.Add(EquipmentSlot[i]);
	}

	UTextBlock* InformationTextBlock[INFORMATIONTEXTBLOCK_MAX]
	{
		Cast<UTextBlock>(GetWidgetFromName(TEXT("Ability_HP_TextBlock"))),
		Cast<UTextBlock>(GetWidgetFromName(TEXT("Ability_Attack_TextBlock"))),
		Cast<UTextBlock>(GetWidgetFromName(TEXT("Ability_DEF_TextBlock"))),
		Cast<UTextBlock>(GetWidgetFromName(TEXT("AttackSpeed_TextBlock"))),
		Cast<UTextBlock>(GetWidgetFromName(TEXT("STR_TextBlock"))),
		Cast<UTextBlock>(GetWidgetFromName(TEXT("Ability_DEX_TextBlock"))),
	};

	for (int i = 0; i < INFORMATIONTEXTBLOCK_MAX; i++)
		InformationText.Add(InformationTextBlock[i]);


	// 캐릭터에서 초기화가 안된다


	if (Data != nullptr)
	{
		if (Data->GetLoadData())
			Data->LoadGameData();
		if (Character->bIsMoveLevel == true)
		{
			Character->GetInventory()->SetMaxInventoryItem(InventortPanel->GetChildrenCount());
			Data->LoadPCDataOpenLevel(Character);
			Data->LoadDataToFalse();
			CInventory* PlayerInventory = Character->GetInventory();
			if (PlayerInventory != nullptr)
			{
				for (int z = 0, y = 0; y < QuickSlotWidget.Num(); z++)
				{
					if (z >= InventortPanel->GetChildrenCount())
						break;
					//if (PlayerInventory->GetItem()[z] != nullptr)
					{
						CItem* ItemData = PlayerInventory->GetItem();
						if (ItemData != nullptr)
						{
							if (ItemData[z].GetQuickSlotSettingID() != 0)
							{
								QuickSlotWidget.GetData()[ItemData[z].GetQuickSlotSettingID() - 1]->SetSlotItemData(&ItemData[z]);
								QuickSlotWidget.GetData()[ItemData[z].GetQuickSlotSettingID() - 1]->UpdataWidget();
								y++;
							}
						}
					}
				}
			}
			if (Character->GetEquipmet()->GetEquipmentItem(EquipmentItemKinds::E_Weapone)->GetItemInformation()->nItemID != 0)
				EquipmentSlotWidget.GetData()[0]->SetSlotItemData(Character->GetEquipmet()->GetEquipmentItem(EquipmentItemKinds::E_Weapone));
			if (Character->GetEquipmet()->GetEquipmentItem(EquipmentItemKinds::E_Armor)->GetItemInformation()->nItemID != 0)
				EquipmentSlotWidget.GetData()[1]->SetSlotItemData(Character->GetEquipmet()->GetEquipmentItem(EquipmentItemKinds::E_Armor));
			if (Character->GetbIsDeath())
			{
				Character->SetbIsDeath(false);
				Character->GetAbility()->nHP = Character->GetAbility()->nMaxHP;
			}
			Character->OnHPChange.Broadcast();
		}
		else
		{

			APOPULGameMode* GameMode = (APOPULGameMode*)UGameplayStatics::GetGameMode((UObject*)GetWorld());
			CItem ItemSword = GameMode->GetItemData((int32)EItemName::E_NORMALSWORD);
			ItemSword.GetItemInformation()->nCount = 1;
			CItem ItemArmor = GameMode->GetItemData((int32)EItemName::E_NORMALARMOR);
			ItemArmor.GetItemInformation()->nCount = 1;
			Character->AddItem(&ItemSword, this);
			Character->AddItem(&ItemArmor, this);

			Character->UpDataAbility();
		}
	}

}

void UDefualtUserWidget::Tick(FGeometry MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);


}

void UDefualtUserWidget::UpdataHP()
{
		APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Character->GetAbility()->nHP == m_nHPBuffer)
			return;
		if (Character != nullptr)
		{
			if (HPText->IsVisible())
			{
				FString StringHP = UKismetStringLibrary::Conv_IntToString(Character->GetAbility()->nHP);
				FString StringMaxHP = UKismetStringLibrary::Conv_IntToString(Character->GetAbility()->nMaxHP);
				FString StringBuffer = UKismetStringLibrary::Concat_StrStr(StringHP, L" / ");
				StringBuffer = UKismetStringLibrary::Concat_StrStr(StringBuffer, StringMaxHP);
				HPText->SetText(UKismetTextLibrary::Conv_StringToText(StringBuffer));
				// 콤마가 적용됨 ex 123,456,789
				//HPText->SetText(UKismetTextLibrary::Conv_IntToText(Character->nHP));
			}
			if (HPProgressBar == nullptr)
				return;
			if (HPProgressBar->IsVisible())
			{
				float Percent = (float)((float)Character->GetAbility()->nHP / (float)Character->GetAbility()->nMaxHP);
				HPProgressBar->SetPercent(Percent);
			}
			m_nHPBuffer = Character->GetAbility()->nHP;
		}
	
}

void UDefualtUserWidget::UpdataInventory()
{
	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character != nullptr)
	{
		if (MoneyText != nullptr)
		{
			if (MoneyText->IsVisible())
			{
				int Monmey = Character->GetInventory()->GetMoney();
				MoneyText->SetText(UKismetTextLibrary::Conv_IntToText(Monmey));
			}
		}
	}
}

void UDefualtUserWidget::CreateWidetOpen()
{
	APOPULGameMode* GameMode = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	CItem Item;

	int i = 0;
	int j = 0;
	for (i = 0;; i++)
	{

		if (GameMode->GameDataNullCheck(i + 1) == true)
			break;
		Item = GameMode->GetItemData(i + 1);
		if (Item.GetItemInformation()->Create.bIsCreate == true)
		{
			CreateItemListSlot[j]->SetSlotItemData_Dummy(Item);
			CreateItemListSlot[j]->SetInventoryNumber(j);
			j++;
		}
	}



	

}

void UDefualtUserWidget::OKboxCheckEnemy()
{
	AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UOKBoxUserWidget* OKBox = Cast<UOKBoxUserWidget>(HUD->__GetOkBoxWidget());
	UInventorySlotUserWidget* Buffer = Cast<UInventorySlotUserWidget>(EnemyInventortPanel->GetChildAt(m_nButtonHoverNumber));
	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AEnemyBaseCharacter* ECharacter = Cast<AEnemyBaseCharacter>(Character->GetOverlapActor());
	if (Buffer != nullptr)
	{
		if (Buffer->GetItemData() != nullptr)
		{
			if (Buffer->GetItemData()->GetItemInformation()->nItemID != 0)
			{
			
				OKBox->GetCountBoxBorder()->SetVisibility(ESlateVisibility::Visible);
				OKBox->SetOKEnemyItemClick();
				FText text = UKismetTextLibrary::Conv_IntToText(Buffer->GetItemData()->GetItemInformation()->nGold);
				OKBox->GetTotalGoldText()->SetText(text);
				if (ECharacter != nullptr)
				{
					switch (ECharacter->GetbIsActorType())
					{
					case ActorType::E_Enemy:
					case ActorType::E_BOX:
						OKBox->OKTextSetText(UKismetTextLibrary::Conv_StringToText(L"가져가시겠습니까?"));
						OKBox->SetCountWonVisible(ESlateVisibility::Hidden);
						break;
					case ActorType::E_NPC:
						OKBox->OKTextSetText(UKismetTextLibrary::Conv_StringToText(L"구매하시겠습니까?"));
						OKBox->SetCountWonVisible(ESlateVisibility::Visible);
						break;
					}
				}
				m_nButtonHoverNumberBuffer = m_nButtonHoverNumber;
				m_nInventoryNumber = Buffer->GetInventoryNumber();
				OKBox->GetCountEditableText()->SetKeyboardFocus();
			}
		}
	}
}

void UDefualtUserWidget::OKboxCheckPC()
{
	AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UOKBoxUserWidget* OKBox = Cast<UOKBoxUserWidget>(HUD->__GetOkBoxWidget());
	UInventorySlotUserWidget* Buffer = Cast<UInventorySlotUserWidget>(InventortPanel->GetChildAt(m_nButtonHoverNumber));
	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	APOPULGameMode* GemeMode = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//퀵슬롯 아이템등록
	if (m_bisQuickSlotSet)
	{
		if (Buffer != nullptr)
		{
			if (Buffer->GetItemData() != nullptr)
			{
				if (Buffer->GetItemData()->GetItemInformation()->nItemID != 0)
				{
					if (Buffer->GetItemData()->GetItemInformation()->Itemtype == Itemtype::E_CONSUMPTION)
					{
						QuickSlotWidgetClickBuffer->SetSlotItemData(Buffer->GetItemData());
						Buffer->GetItemData()->SetQuickSlotSettingID(QuickSlotWidgetClickBuffer->GetQuickSlotID());
						QuickSlotWidgetClickBuffer->UpdataWidget();
						QuickSlotWidgetClickBuffer = nullptr;
						m_bisQuickSlotSet = false;
						return;
					}
				}
			}
		}
		else
		{
			if (QuickSlotWidgetClickBuffer != nullptr)
			{
				if (QuickSlotWidgetClickBuffer->GetItemData() != nullptr)
				{
					CItem* ItemBuffer = QuickSlotWidgetClickBuffer->GetItemData();
					CInventory* PlayerInventory = Character->GetInventory();
					if (ItemBuffer != nullptr)
					{
						for (int z = 0; z < InventortPanel->GetChildrenCount(); z++)
						{
							if (QuickSlotWidgetClickBuffer->GetQuickSlotID() == PlayerInventory->GetItem()[z].GetQuickSlotSettingID())
								PlayerInventory->GetItem()[z].SetQuickSlotSettingID(0);
						}
					}
				}
			}
			QuickSlotWidgetClickBuffer->EmptyItemData();
			QuickSlotWidgetClickBuffer = nullptr;
			m_bisQuickSlotSet = false;
			return;
		}
	}
	// 아이템 장착 장착템 만들고 확인할것
	else if (m_bIsEquipmnetSlotSet)
	{
		if (Buffer != nullptr)
		{
			if (Buffer->GetItemData() != nullptr)
			{
				if (Buffer->GetItemData()->GetItemInformation()->nItemID != 0)
				{
					switch (Buffer->GetItemData()->GetItemInformation()->Itemtype)
					{
					case Itemtype::E_EQUIPMENT_WEAPONE:
						if (EquipmentSlotWidgetClickBuffer->GetEquipmentSlotNumber() != EquipmentItemKinds::E_Weapone)
							return;
						if (EquipmentSlotWidgetClickBuffer->GetItemData() != nullptr)
						{
							EquipmentSlotWidgetClickBuffer->GetItemData()->GetItemInformation()->nCount = 1;
							Character->AddItem(EquipmentSlotWidgetClickBuffer->GetItemData(), this, ActorType::E_PC);
							EquipmentSlotWidgetClickBuffer->EmptyItemData();
						}
						Character->GetEquipmet()->GetEquipmentItem(EquipmentItemKinds::E_Weapone)->SetItem(Buffer->GetItemData());
						EquipmentSlotWidgetClickBuffer->SetSlotItemData(Character->GetEquipmet()->GetEquipmentItem(EquipmentItemKinds::E_Weapone));
						GemeMode->UsingItem_Ex(Buffer->GetItemData(), Character);
						UpDataWidget();
						EquipmentSlotWidgetClickBuffer = nullptr;
						m_bIsEquipmnetSlotSet = false;
						return;
					case Itemtype::E_EQUIPMENT_ARMOR:
						if (EquipmentSlotWidgetClickBuffer->GetEquipmentSlotNumber() != EquipmentItemKinds::E_Armor)
							return;
						if (EquipmentSlotWidgetClickBuffer->GetItemData() != nullptr)
						{
							EquipmentSlotWidgetClickBuffer->GetItemData()->GetItemInformation()->nCount = 1;
							Character->AddItem(EquipmentSlotWidgetClickBuffer->GetItemData(), this, ActorType::E_PC);
							EquipmentSlotWidgetClickBuffer->EmptyItemData();
						}
						Character->GetEquipmet()->GetEquipmentItem(EquipmentItemKinds::E_Armor)->SetItem(Buffer->GetItemData());
						EquipmentSlotWidgetClickBuffer->SetSlotItemData(Character->GetEquipmet()->GetEquipmentItem(EquipmentItemKinds::E_Armor));
						GemeMode->UsingItem_Ex(Buffer->GetItemData(), Character);
						UpDataWidget();
						EquipmentSlotWidgetClickBuffer = nullptr;
						m_bIsEquipmnetSlotSet = false;
						return;
					default:
						break;
					}
					
				}
			}
		}
		else
		{
			// 빈공간 클릭시 아무일도 안일어나게만듦 추후에 변경
			EquipmentSlotWidgetClickBuffer = nullptr;
			m_bIsEquipmnetSlotSet = false;
			return;

			// 번경 내용 아이템 해제하는 코드 없음
			if (EquipmentSlotWidgetClickBuffer != nullptr)
			{
				if (EquipmentSlotWidgetClickBuffer->GetItemData() != nullptr)
				{
					CItem* ItemBuffer = EquipmentSlotWidgetClickBuffer->GetItemData();
					CInventory* PlayerInventory = Character->GetInventory();
					if (ItemBuffer != nullptr)
					{
						Character->AddItem(ItemBuffer, this, ActorType::E_PC);
						UpDataWidget();
					}
				}
			}
			EquipmentSlotWidgetClickBuffer->EmptyItemData();
			EquipmentSlotWidgetClickBuffer = nullptr;
			m_bIsEquipmnetSlotSet = false;
			return;
		}
	}

	// 기본 인벤토리아이템 사용
	if (Buffer != nullptr)
	{
		if (Buffer->GetItemData() != nullptr)
		{
			if (Buffer->GetItemData()->GetItemInformation()->nItemID != 0)
			{
				AEnemyBaseCharacter* ECharacer = Cast<AEnemyBaseCharacter>(Character->GetOverlapActor());
				OKBox->SetCountWonVisible(ESlateVisibility::Visible);
				switch (Buffer->GetItemData()->GetItemInformation()->Itemtype)
				{
				case Itemtype::E_CONSUMPTION:
					if (Character->bIsOverlapActorEnemyBaseCharacter() == false || EnemyInventoryBorder->IsVisible() == false)
					{
						OKBox->OKTextSetText(UKismetTextLibrary::Conv_StringToText(L"사용하시겠습니까?"));
						OKBox->GetOKBoxBorder()->SetVisibility(ESlateVisibility::Visible);
						OKBox->SetOKPCItemClick();
						break;
					}
				default:
				{
					switch (Character->OverlapActorTypeFind())
					{
					case ActorType::E_NPC:
						OKBox->OKTextSetText(UKismetTextLibrary::Conv_StringToText(L"판매하시겠습니까?"));
						break;
					case ActorType::E_BOX:
					case ActorType::E_Enemy:
						OKBox->OKTextSetText(UKismetTextLibrary::Conv_StringToText(L"넣으시겠습니까?"));
						OKBox->SetCountWonVisible(ESlateVisibility::Hidden);
						break;
					default:
					case ActorType::E_NONE:
						return;

					}
					OKBox->GetCountBoxBorder()->SetVisibility(ESlateVisibility::Visible);
					OKBox->SetOKPCItemClick();
					FText text = UKismetTextLibrary::Conv_IntToText(Buffer->GetItemData()->GetItemInformation()->nGold);
					OKBox->GetTotalGoldText()->SetText(text);
					OKBox->GetCountEditableText()->SetKeyboardFocus();
				}
				}
				m_nButtonHoverNumberBuffer = m_nButtonHoverNumber;
			}

		}
	}
}

void UDefualtUserWidget::InvenClickedFunction()
{
	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UOKBoxUserWidget* OKBox = Cast<UOKBoxUserWidget>(HUD->__GetOkBoxWidget());
	UInventorySlotUserWidget* SlotBuffer = nullptr;

	if (Character == nullptr)
		return;
	if (Character->GetbIsKeyDownQuitGame() == true)
	{
		APOPULGameMode* GameMode = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->QuitGame();
		return;
	}

	{
		if (OKBox->GetEnemyItemClick() == true)
		{
			if (m_nButtonHoverNumberBuffer != -1)
				UDefualtUserWidget::EnemyInventoryCilckFunction(Character, HUD);
			return;
		}
		else if (OKBox->GetPCItemClick() == true)
		{
			if (m_nButtonHoverNumberBuffer != -1)
				UDefualtUserWidget::PCInventoryCilckFunction(Character, HUD);
			return;
		}
	}

	

}



void UDefualtUserWidget::EnemyInventoryCilckFunction(APOPULCharacter* Character , AMainHUD* HUD)
{
	UInventorySlotUserWidget* SlotBuffer = Cast<UInventorySlotUserWidget>(EnemyInventortPanel->GetChildAt(m_nButtonHoverNumberBuffer));
	UOKBoxUserWidget* OK = Cast<UOKBoxUserWidget>(HUD->__GetOkBoxWidget());
	if (SlotBuffer->GetItemData()->GetItemInformation()->nItemID != 0)
	{
		CItem* Item = SlotBuffer->GetItemData();
		AEnemyBaseCharacter* EActor = Cast<AEnemyBaseCharacter>(Character->GetOverlapActor());
		if (EActor != nullptr)
		{
			if (EActor->GetbIsActorType() == ActorType::E_NPC)
			{

				int TotalMoney = Item->GetItemInformation()->nGold * m_ItemCountNumber;
				if (Character->GetInventory()->GetMoney() < TotalMoney)
				{
					if (OK != nullptr)
						OK->SetMessageBox(UKismetTextLibrary::Conv_StringToText(L"금액이 부족합니다"));
					// 금액부족 메시지 띄우기
					return;
				}
				Character->AddMoney(-1 * TotalMoney);
			}

			int CountBuffer = Item->GetItemInformation()->nCount;
			Item->GetItemInformation()->nCount = m_ItemCountNumber;
			Character->AddItem(Item, HUD->__GetDefualtWidget());
			Item->GetItemInformation()->nCount = CountBuffer;
			Item->GetItemInformation()->nCount -= m_ItemCountNumber;

			if (Item->GetItemInformation()->nCount <= 0)
				EActor->GetInventory()->EmptyItem(SlotBuffer->GetInventoryNumber());
			SlotBuffer->UpdataWidget();
		}
		
			
			//
	}
	
}

void UDefualtUserWidget::PCInventoryCilckFunction(APOPULCharacter* Character, AMainHUD* HUD)
{
	UInventorySlotUserWidget* SlotBuffer = Cast<UInventorySlotUserWidget>(InventortPanel->GetChildAt(m_nButtonHoverNumberBuffer));

	if (SlotBuffer->GetItemData()->GetItemInformation()->nItemID != 0)
	{
		CItem* Item = SlotBuffer->GetItemData();
		AEnemyBaseCharacter* EActor = Cast<AEnemyBaseCharacter>(Character->GetOverlapActor());
		if (EActor != nullptr && 
			EnemyInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		{
			if (EnemyInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
			{
				int Money = Item->GetItemInformation()->nGold * m_ItemCountNumber;
				int CountBuffer = Item->GetItemInformation()->nCount;
				Item->GetItemInformation()->nCount = m_ItemCountNumber;
				EActor->GetInventory()->AddItem(Item, HUD->__GetDefualtWidget(), ActorType::E_Enemy);
				if (Character->OverlapActorTypeFind() == ActorType::E_NPC)
					Character->AddMoney(Money);
				Item->GetItemInformation()->nCount = CountBuffer;
				Item->GetItemInformation()->nCount -= m_ItemCountNumber;
				if (Item->GetItemInformation()->nCount <= 0)
					Character->GetInventory()->EmptyItem(m_nButtonHoverNumberBuffer);
				SlotBuffer->UpdataWidget();
				UpDataWidget();
			}
		}
		else 
		{
			int nItmetype = SlotBuffer->GetItemData()->GetItemInformation()->Itemtype;
			if (nItmetype == Itemtype::E_CONSUMPTION)
			{
				APOPULGameMode* GameMovd = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				GameMovd->UsingItem_Ex(Item, Character);

			}

		}
	}
}

// 제작리스트 클릭
void UDefualtUserWidget::CreateItemListCheck()
{
	APOPULGameMode* GameMode = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UInventorySlotUserWidget* ItemSlot = nullptr;
	CItem Item;
	int i = 0, j = 0;

	
	if (CreateItemListSlot[m_nCreateHoverNumber] == nullptr || m_nCreateHoverNumber == -1)
		return;
	InformationSlot->SetSlotItemData_Dummy(*CreateItemListSlot[m_nCreateHoverNumber]->GetItemData());
	int CreateMaterialItemID[CREATEMATERIAL_MAX]
	{
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID1,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID2,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID3,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID4,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID5,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID6,
	};
	int CreateMaterialItemCount[CREATEMATERIAL_MAX]
	{
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount1,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount2,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount3,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount4,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount5,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount6,
	};

	
	
	FString Name = CreateItemListSlot[m_nCreateHoverNumber]->GetItemData()->GetName();
	FString Infomation = CreateItemListSlot[m_nCreateHoverNumber]->GetItemData()->GetInfomationText();
	Infomation = UItemToolTipUserWidget::Newline(Infomation , 9);
	NameText->SetText(UKismetTextLibrary::Conv_StringToText(Name));
	IteminformationText->SetText(UKismetTextLibrary::Conv_StringToText(Infomation));
	int HP = InformationSlot->GetItemData()->GetItemInformation()->Create.nCreateUsingHP;
	UsingHPText->SetText(FText::FromString(UKismetStringLibrary::Conv_IntToString(HP)));
	for (i = 0; i < CREATEMATERIAL_MAX; i++)
	{
		ItemSlot = Cast<UInventorySlotUserWidget>(MaterialItemPanel->GetChildAt(i));
		ItemSlot->EmptyItemData();
		if (CreateMaterialItemID[i] == 0)
			continue;
		Item = GameMode->GetItemData(CreateMaterialItemID[i]);

		ItemSlot->SetSlotItemData_Dummy(Item, CreateMaterialItemCount[i]);
	}
	
	m_nButtonHoverNumberBuffer = m_nCreateHoverNumber;
}

// 크리에이트 버튼
void UDefualtUserWidget::CreateButtonCheck()
{
	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UOKBoxUserWidget* OK = Cast<UOKBoxUserWidget>(HUD->__GetOkBoxWidget());
	if (InformationSlot->GetItemData() == nullptr)
		return;
	int HP = Character->GetAbility()->nHP - InformationSlot->GetItemData()->GetItemInformation()->Create.nCreateUsingHP;
	if (HP <= 0)
	{
		OK->SetMessageBox(FText::FromString(L"체력이 부족합니다"));
		return;
	}
	int InventoryNumber[CREATEMATERIAL_MAX];
	int CreateMaterialItemID[CREATEMATERIAL_MAX]
	{
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID1,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID2,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID3,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID4,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID5,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialItemID6,
	};
	int CreateMaterialItemCount[CREATEMATERIAL_MAX]
	{
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount1,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount2,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount3,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount4,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount5,
		InformationSlot->GetItemData()->GetItemInformation()->Create.nmaterialCount6,
	};
	int i = 0;

	for (i = 0; i < CREATEMATERIAL_MAX; i++)
	{
		if (CreateMaterialItemID[i] == 0)
		{
			InventoryNumber[i] = -3;
			continue;
		}
		InventoryNumber[i] = Character->GetInventory()->FindItem_CountCheck(CreateMaterialItemID[i], CreateMaterialItemCount[i]);
		if (InventoryNumber[i] < 0)
		{
			OK->SetMessageBox(FText::FromString(L"재료가 부족합니다"));
			return;
		}
	}


	for (i = 0; i < CREATEMATERIAL_MAX; i++)
	{
		if (InventoryNumber[i] == -3)
			continue;


		Character->GetInventory()->GetItem()[InventoryNumber[i]].GetItemInformation()->nCount -= CreateMaterialItemCount[i];
		Character->GetInventory()->GetItem()[InventoryNumber[i]].GetItemInformation()->ItemWidget->UpdataWidget();
		if (Character->GetInventory()->GetItem()[InventoryNumber[i]].GetItemInformation()->nCount <= 0)
			Character->GetInventory()->EmptyItem(InventoryNumber[i]);
	}

	
	CItem Item = *InformationSlot->GetItemData();
	Item.GetItemInformation()->nCount = Item.GetItemInformation()->Create.nReturnCount;
	Character->GetInventory()->AddItem(&Item, this);
	Character->AddHP(-InformationSlot->GetItemData()->GetItemInformation()->Create.nCreateUsingHP);
}

// 대사 버튼

void UDefualtUserWidget::DialogNoButtonClick()
{
	DialogBorder->SetVisibility(ESlateVisibility::Hidden);
	UDefualtUserWidget::VisibilityDialog(ESlateVisibility::Hidden);
	m_nDialogNumber = 1;
	DialogStr = nullptr;
}

void UDefualtUserWidget::DialogActionButtonClick()
{
	if (m_bIsNextDialog == false)
	{
		EnemyInventoryBorder->SetVisibility(ESlateVisibility::Visible);
		UTextBlock* Text = Cast<UTextBlock>(EnemyInventoryHeaderBorder->GetChildAt(0));
		Text->SetText(FText::FromString(DialogStr[0]));
		UDefualtUserWidget::VisibilityDialog(ESlateVisibility::Hidden);
		m_nDialogNumber = 1;
		DialogStr = nullptr;
		return;
	}
	UDefualtUserWidget::NextDialog();


}

void UDefualtUserWidget::VisibilityDialog(ESlateVisibility Visibility)
{
	bool bIsNextDialog = false;
	DialogBorder->SetVisibility(Visibility);
	DialogButtonAction->SetVisibility(Visibility);
	DialogButtonNo->SetVisibility(Visibility);

}

void UDefualtUserWidget::VisibilityDialog(ESlateVisibility Visibility, DailogName Name)
{

	UDefualtUserWidget::VisibilityDialog(Visibility);
	APOPULGameMode* GameMode = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	DialogStr = GameMode->GetDialog(strDailogName[Name]);
	m_nDialogNumber = 1;
	if (DialogStr != nullptr)
	{
		NPCNameText->SetText(FText::FromString(DialogStr[0]));
		FString Dialog = UItemToolTipUserWidget::Newline(DialogStr[m_nDialogNumber], DIALOGLINE_MAX);
		NPCDialogText->SetText(FText::FromString(Dialog));
		UDefualtUserWidget::NextDialogChcek(DialogStr, m_nDialogNumber);
	}
}

void UDefualtUserWidget::DialogSetting(int DialogNumber)
{
	if (DialogStr == nullptr)
		return;
	UDefualtUserWidget::NextDialogChcek(DialogStr, DialogNumber);
	FString Dialog = UItemToolTipUserWidget::Newline(DialogStr[DialogNumber], DIALOGLINE_MAX);
	NPCDialogText->SetText(FText::FromString(Dialog));
}

void UDefualtUserWidget::NextDialog()
{
	if (m_nDialogNumber == 0)
		m_nDialogNumber = 1;
	m_nDialogNumber++;
	UDefualtUserWidget::DialogSetting(m_nDialogNumber);
}

bool UDefualtUserWidget::NextDialogChcek(FString* Str, int NowNumber)
{
	m_bIsNextDialog = false;
	UBorder* Border = Cast<UBorder>(DialogButtonAction->GetChildAt(0));
	UTextBlock* Text =Cast<UTextBlock>(Border->GetChildAt(0));
	for (int i = NowNumber; i < DIALOG_MAX; i++)
	{
		if (NowNumber + 1 == DIALOG_MAX || Str[i + 1].IsEmpty())
		{
			
			Text->SetText(FText::FromString(L"상   점"));
			return true;
		}
		if (!Str[i + 1].IsEmpty())
		{
			
			Text->SetText(FText::FromString(L"다   음"));
			m_bIsNextDialog = true;
			return false;
		}
	}

	Text->SetText(FText::FromString(L"상   잠"));
	return true;
}


#define UIMax 4 

void UDefualtUserWidget::QuitUIAndGame()
{
	UBorder* UIBorder[UIMax]
	{
		CreateBorder,
		EnemyInventoryBorder,
		InventoryBorder,
		InformationBorder,
	};

	int32 UIZOrder[UIMax]
	{
		CreateBordercanvasPanelSlot->GetZOrder(),
		EnmeyBordercanvasPanelSlot->GetZOrder(),
		BordercanvasPanelSlot->GetZOrder(),
		InformationBordercanvasPanelSlot->GetZOrder(),
	};

	APOPULCharacter* Charcter = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	bool bIsZero = false;
	int Count = 0;
	while (true)
	{
		Count = 0;
		for (int i = 0; i < UIMax; i++)
		{
			if (UIBorder[i]->GetVisibility() == ESlateVisibility::Hidden)
				Count++;
			if (bIsZero == false)
			{
				if (UIZOrder[i] == (int32)1)
				{
					if (UIBorder[i]->GetVisibility() == ESlateVisibility::Visible)
					{
						UIBorder[i]->SetVisibility(ESlateVisibility::Hidden);
						switch (i)
						{
						case 2:
							Charcter->OpenInventory();
							break;
						case 3:
							Charcter->OpenInformation();
							break;
						}
						bIsZero = false;
						break;
					}
				}
			}
			else
			{
				if (UIBorder[i]->GetVisibility() == ESlateVisibility::Visible)
				{
					UIBorder[i]->SetVisibility(ESlateVisibility::Hidden);
					switch (i)
					{
					case 2:
						Charcter->OpenInventory();
						break;
					case 3:
						Charcter->OpenInformation();
						break;
					}
					bIsZero = false;
					break;
				}
			}
			bIsZero = true;
		}
		if (bIsZero == false || Count == UIMax)
			break;
	}
	if (Count == UIMax)
	{
		
		Charcter->QuitGame();
	}
}

void UDefualtUserWidget::UpDataWidget()
{
	if (InventortPanel != nullptr)
	{
		APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		
		UInventorySlotUserWidget* Buffer = nullptr;// = Cast<UInventorySlotUserWidget>(InventortPanel->GetChildAt(0));
		UUniformGridSlot* InventortSlot = nullptr;//
		for (int i = 0; i < InventortPanel->GetChildrenCount(); i++)
		{
			if (InventortPanel->GetChildAt(i) == nullptr)
				continue;
			Buffer = Cast<UInventorySlotUserWidget>(InventortPanel->GetChildAt(i));
			if (Buffer->GetItemButton() != nullptr)
			{
				if (Buffer->GetItemData() != nullptr &&
					Buffer->GetItemData()->GetItemInformation() != nullptr)
				{
					if (Buffer->GetItemData()->GetItemInformation()->nCount <= 0)
					{
						Buffer->GetItemData()->GetItemInformation()->nItemID = 0;
						Buffer->EmptyItemData();
					}
				}
				Buffer->UpdataWidget();
			}
		}
	}

	for (int i = 0; i < QuickSlotWidget.Num(); i++)
	{
		UInventorySlotUserWidget* QuickSlot = QuickSlotWidget.GetData()[i];
		if (QuickSlot != nullptr)
		{
			CItem* Item = QuickSlot->GetItemData();
			if (Item != nullptr)
			{
				if (QuickSlot->GetItemData()->GetItemInformation()->nCount <= 0)
					QuickSlot->EmptyItemData();
			}
			QuickSlot->UpdataWidget();
		}
	}

	UDefualtUserWidget::UpdataInformationText();
}

// 퀵슬롯
void UDefualtUserWidget::QuickSlotButtonClick()
{
	m_bisQuickSlotSet = true;
	m_bIsEquipmnetSlotSet = false;
	QuickSlotWidgetClickBuffer = QuickSlotWidgetBuffer;
}

void UDefualtUserWidget::SetHoverQuickSlotNum(UUserWidget* Widget)
{
	if (Widget != nullptr)
		QuickSlotWidgetBuffer = Cast<UInventorySlotUserWidget>(Widget);
	else
		QuickSlotWidgetBuffer = nullptr;
}

// 정보 UI 함수

void UDefualtUserWidget::UpdataInformationText()
{
	APOPULCharacter* PlayerCharacter = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter != nullptr)
	{
		Ability* PlayerAbility = PlayerCharacter->GetAbility();
		if (PlayerAbility != nullptr)
		{
			if (InformationText.GetData() == nullptr)
				return;
			FString PretixInformationString[INFORMATIONTEXTBLOCK_MAX]
			{
				((TEXT("    체 력   : "))),
				((TEXT("  공 격 력  : "))),
				((TEXT("  방 어 력  : "))),
				((TEXT("공 격 속 도 : "))),
				((TEXT("     힘     : "))),
				((TEXT("   민  첩   : "))),

			};
			int i = 0;
			FText ConcatTextMaxHP = FText::FromString(UKismetStringLibrary::Concat_StrStr((TEXT("    체 력   : ")), UKismetStringLibrary::Conv_IntToString(PlayerAbility->nMaxHP)));
			InformationText.GetData()[i++]->SetText(ConcatTextMaxHP);

			FText ConcatTextAttack = FText::FromString(UKismetStringLibrary::Concat_StrStr((TEXT("  공 격 력  : ")), UKismetStringLibrary::Conv_IntToString(PlayerAbility->nAttack)));
			InformationText.GetData()[i++]->SetText(ConcatTextAttack);

			FText ConcatTextDefend = FText::FromString(UKismetStringLibrary::Concat_StrStr((TEXT("  방 어 력  : ")), UKismetStringLibrary::Conv_IntToString(PlayerAbility->nDefend)));
			InformationText.GetData()[i++]->SetText(ConcatTextDefend);

			FText ConcatTextAttackSpeed = FText::FromString(UKismetStringLibrary::Concat_StrStr((TEXT("공 격 속 도 : ")), UKismetStringLibrary::Conv_FloatToString(PlayerAbility->nAttackSpeed)));
			if (InformationText.GetData()[i]->IsVisible())
				InformationText.GetData()[i++]->SetText(ConcatTextAttackSpeed);

			FText ConcatTextSTR = FText::FromString(UKismetStringLibrary::Concat_StrStr((TEXT("     힘     : ")), UKismetStringLibrary::Conv_IntToString(PlayerAbility->nSTR)));
			InformationText.GetData()[i++]->SetText(ConcatTextSTR);

			FText ConcatTextDEX = FText::FromString(UKismetStringLibrary::Concat_StrStr((TEXT("   민  첩   : ")), UKismetStringLibrary::Conv_IntToString(PlayerAbility->nDEX)));
			InformationText.GetData()[i++]->SetText(ConcatTextDEX);
				
		}
	}
}

void UDefualtUserWidget::SetHoverEquipmentSlot(UUserWidget* Widget)
{
	if (Widget != nullptr)
		EquipmentSlotWidgetBuffer = Cast<UInventorySlotUserWidget>(Widget);
	else
		EquipmentSlotWidgetBuffer = nullptr;
}

void UDefualtUserWidget::EquipmentClick()
{
	m_bisQuickSlotSet = false;
	m_bIsEquipmnetSlotSet = true;
	EquipmentSlotWidgetClickBuffer = EquipmentSlotWidgetBuffer;
	//EquipmentSlot;
}

// 마우스 인벤조작

FEventReply UDefualtUserWidget::OnMouseButtonDown1(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::OnMouseButtonDown(MyGeometry,  MouseEvent);

	m_isMouseButtonDown = true;
	FVector2D MousePosition(0.0f, 0.0f);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	PlayerController->SetInputMode(FInputModeGameAndUI());
	LocationBuffer = MousePosition;

	return FEventReply(false);
}

// 인벤토리 UI 움직임 함수
FEventReply UDefualtUserWidget::OnMouseMove1(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::OnMouseMove(MyGeometry, MouseEvent);
	FVector2D MousePosition(0.0f,0.0f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePosition.X, MousePosition.Y);
	if (m_isMouseButtonDown == true)
	{
		FVector2D Buffer = MousePosition - LocationBuffer;
		Buffer.X *= 1.5f;
		Buffer.Y *= 1.5f;
		fvInventoyWidgetLocation += Buffer;
		BordercanvasPanelSlot->SetPosition(fvInventoyWidgetLocation);
		BordercanvasPanelSlot->SetZOrder(1);
		InformationBordercanvasPanelSlot->SetZOrder(0);
		CreateBordercanvasPanelSlot->SetZOrder(0);
		EnmeyBordercanvasPanelSlot->SetZOrder(0);
		LocationBuffer = MousePosition;
	}
	return FEventReply(false);
}

FEventReply UDefualtUserWidget::OnMouseButtonUp1(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::OnMouseButtonUp(MyGeometry, MouseEvent);
	if (m_isMouseButtonDown == true)
	{
		m_isMouseButtonDown = false;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameAndUI());
	}
	return FEventReply(false);
}


// 제작 UI 움직임 함수
FEventReply UDefualtUserWidget::OnMouseMoveCreate(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::OnMouseMove(MyGeometry, MouseEvent);
	FVector2D MousePosition(0.0f, 0.0f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePosition.X, MousePosition.Y);
	if (m_isMouseButtonDown == true)
	{
		FVector2D Buffer = MousePosition - LocationBuffer;
		Buffer.X *= 1.5f;
		Buffer.Y *= 1.5f;
		fvCreateWidgetLocation += Buffer;
		CreateBordercanvasPanelSlot->SetPosition(fvCreateWidgetLocation);
		CreateBordercanvasPanelSlot->SetZOrder(1);
		InformationBordercanvasPanelSlot->SetZOrder(0);
		EnmeyBordercanvasPanelSlot->SetZOrder(0);
		BordercanvasPanelSlot->SetZOrder(0);
		LocationBuffer = MousePosition;
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, UKismetStringLibrary::Conv_Vector2dToString(fvCreateWidgetLocation));
	}
	return FEventReply(false);
}

// 적인벤토리 UI 움직임 함수
FEventReply UDefualtUserWidget::OnMouseMoveEnemy(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::OnMouseMove(MyGeometry, MouseEvent);
	FVector2D MousePosition(0.0f, 0.0f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePosition.X, MousePosition.Y);
	if (m_isMouseButtonDown == true)
	{
		FVector2D Buffer = MousePosition - LocationBuffer;
		Buffer.X *= 1.5f;
		Buffer.Y *= 1.5f;
		fvEnemyWidgetLocation += Buffer;
		EnmeyBordercanvasPanelSlot->SetPosition(fvEnemyWidgetLocation);
		EnmeyBordercanvasPanelSlot->SetZOrder(1);
		InformationBordercanvasPanelSlot->SetZOrder(0);
		CreateBordercanvasPanelSlot->SetZOrder(0);
		BordercanvasPanelSlot->SetZOrder(0);
		LocationBuffer = MousePosition;
	}
	return FEventReply(false);
}

// 정보 UI 움직임 함수
FEventReply UDefualtUserWidget::OnMouseMoveInformation(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::OnMouseMove(MyGeometry, MouseEvent);
	FVector2D MousePosition(0.0f, 0.0f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePosition.X, MousePosition.Y);
	if (m_isMouseButtonDown == true)
	{
		FVector2D Buffer = MousePosition - LocationBuffer;
		Buffer.X *= 1.5f;
		Buffer.Y *= 1.5f;
		fvInformationLocation += Buffer;
		InformationBordercanvasPanelSlot->SetPosition(fvInformationLocation);
		InformationBordercanvasPanelSlot->SetZOrder(1);
		EnmeyBordercanvasPanelSlot->SetZOrder(0);
		CreateBordercanvasPanelSlot->SetZOrder(0);
		BordercanvasPanelSlot->SetZOrder(0);
		LocationBuffer = MousePosition;
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, UKismetStringLibrary::Conv_Vector2dToString(fvInformationLocation));
	}
	return FEventReply(false);
}

void UDefualtUserWidget::SettingActionUI(bool Bool)
{
	if (this != nullptr && ActionText != nullptr)
	{
		if (Bool)
			ActionText->SetVisibility(ESlateVisibility::Visible);
		else
			ActionText->SetVisibility(ESlateVisibility::Hidden);
	}
}