// Fill out your copyright notice in the Description page of Project Settings.

#include "OKBoxUserWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "POPULCharacter.h"
#include "EnemyBaseCharacter.h"
#include "DefualtUserWidget.h"
#include "MainHUD.h"
#include "Engine.h"


void UOKBoxUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	YesButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonYes")));
	NoButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonNo")));
	OKBoxBorder = Cast<UBorder>(GetWidgetFromName(TEXT("OKBoxBorder")));
	OKText = Cast<UTextBlock>(GetWidgetFromName(TEXT("OkTextBlock")));
	if (OKText != nullptr)
		OKText->SetText(UKismetTextLibrary::Conv_StringToText(FString(L"실행하시겠습니까?")));
	if (OKBoxBorder != nullptr)
		OKBoxBorder->SetVisibility(ESlateVisibility::Hidden);
	if (YesButton != nullptr)
	{
		YesButton->OnClicked.AddDynamic(this, &UOKBoxUserWidget::YesButtonClickedFunction);
	}
	if (NoButton != nullptr)
	{
		NoButton->OnClicked.AddDynamic(this, &UOKBoxUserWidget::NoButtonClickedFunction);
	}
	
	CountBoxBorder = Cast<UBorder>(GetWidgetFromName(TEXT("CountBorder")));
	if (CountBoxBorder != nullptr)
	{
		CountBoxBorder->SetVisibility(ESlateVisibility::Hidden);
	}
	WonText = Cast<UTextBlock>(GetWidgetFromName(TEXT("WonTextBlock")));
	TotalGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TotalGoldTextBlock")));
	CountYesButton = Cast<UButton>(GetWidgetFromName(TEXT("CountButtonYes")));
	CountNoButton = Cast<UButton>(GetWidgetFromName(TEXT("CountButtonNo")));
	CountEditableText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("CountEditableTextBox")));
	if (CountYesButton != nullptr)
		CountYesButton->OnClicked.AddDynamic(this, &UOKBoxUserWidget::CountYesButtonClickedFunction);
	if (CountNoButton != nullptr)
		CountNoButton->OnClicked.AddDynamic(this, &UOKBoxUserWidget::CountNoButtonClickedFunction);
	if (CountEditableText != nullptr)
	{
		CountEditableText->OnTextChanged.AddDynamic(this,&UOKBoxUserWidget::CountEditText);
		CountEditableText->OnTextCommitted.AddDynamic(this, &UOKBoxUserWidget::CountEditTextCommitted);
		CountEditableText->SetText(UKismetTextLibrary::Conv_IntToText(1));
	}



	MessageButton = Cast<UButton>(GetWidgetFromName(TEXT("MessageOkButton")));
	if (MessageButton != nullptr)
		MessageButton->OnClicked.AddDynamic(this, &UOKBoxUserWidget::MessageButtonClickedFunction);
	MessageBorder = Cast<UBorder>(GetWidgetFromName(TEXT("MessageBorder")));
	if (MessageBorder != nullptr)
		MessageBorder->SetVisibility(ESlateVisibility::Hidden);
	MessageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MessageTextBlock")));;


	m_bIsOK = false;
	m_bIsEnemyItemCilck = false;
	m_bIsPCItemCilck = false;
	m_Count_TotalMoney = 0;
	m_CountEditableText_Count = 1;
	m_CountEditableText_CountBuffer = m_CountEditableText_Count;
}

//엔터키로 없애보아라
void UOKBoxUserWidget::Tick(FGeometry MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
	APlayerController* PCController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PCController->IsInputKeyDown(EKeys::Enter))
	{
		if (CountBoxBorder->IsVisible())
		{
			UOKBoxUserWidget::CountYesButtonClickedFunction();
		}
	}
}

void UOKBoxUserWidget::SetCountdata()
{
	m_CountEditableText_Count = 1;
	m_CountEditableText_CountBuffer = m_CountEditableText_Count;
	CountEditableText->SetText(UKismetTextLibrary::Conv_IntToText(1));
}

void UOKBoxUserWidget::SetCountWonVisible(ESlateVisibility InVisibility)
{
	WonText->SetVisibility(InVisibility);
	TotalGoldText->SetVisibility(InVisibility);
}

void UOKBoxUserWidget::OKTextSetText(const FText& text)
{
	OKText->SetText(text);
}

void UOKBoxUserWidget::SetMessageText(const FText& text)
{
	MessageText->SetText(text);
}

void UOKBoxUserWidget::SetMessageBox(const FText& text)
{
	UOKBoxUserWidget::SetMessageText(text);
	MessageBorder->SetVisibility(ESlateVisibility::Visible);
}


void UOKBoxUserWidget::YesButtonClickedFunction()
{
	m_bIsOK = true;
	OnOkBoxInven.ExecuteIfBound();
	OKBoxBorder->SetVisibility(ESlateVisibility::Hidden);
	m_bIsEnemyItemCilck = false;
	m_bIsPCItemCilck = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, L"yes버튼");
}

void UOKBoxUserWidget::NoButtonClickedFunction()
{
	m_bIsOK = false;
	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Character->SetbIsKeyDownQuitGame(false);
	OKBoxBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UOKBoxUserWidget::CountYesButtonClickedFunction()
{
	AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UDefualtUserWidget* DefaultWidget = Cast<UDefualtUserWidget>(HUD->__GetDefualtWidget());

	DefaultWidget->SetItemCountNumber(m_CountEditableText_Count);
	CountBoxBorder->SetVisibility(ESlateVisibility::Hidden);
	OKBoxBorder->SetVisibility(ESlateVisibility::Visible);
	UOKBoxUserWidget::SetCountdata();
}

void UOKBoxUserWidget::CountNoButtonClickedFunction()
{
	CountBoxBorder->SetVisibility(ESlateVisibility::Hidden);
	m_bIsEnemyItemCilck = false;
	m_bIsPCItemCilck = false;
	UOKBoxUserWidget::SetCountdata();
}

void UOKBoxUserWidget::CountEditText(const FText& Text)
{
	FString Str = UKismetTextLibrary::Conv_TextToString(Text);
	m_CountEditableText_Count = UKismetStringLibrary::Conv_StringToInt(Str);

	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AMainHUD* HUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	int HoveredItemCount = 0;
	int HoveredItemGold = 0;
	if (Character != nullptr && HUD != nullptr)
	{
		UDefualtUserWidget* DefaultWidget = Cast<UDefualtUserWidget>(HUD->__GetDefualtWidget());
		if (DefaultWidget != nullptr)
		{
			int Hovered = DefaultWidget->GetHoveredNumber();//DefaultWidget->GetInventoryNumber();//DefaultWidget->GetHoveredNumber();
			if (m_bIsEnemyItemCilck == true)
			{
				AEnemyBaseCharacter* EnemyCharcater = Cast<AEnemyBaseCharacter>(Character->GetOverlapActor());
				if (EnemyCharcater == nullptr)
					return;
				HoveredItemCount = EnemyCharcater->GetInventory()->GetItem()[Hovered].GetItemInformation()->nCount;
				HoveredItemGold = EnemyCharcater->GetInventory()->GetItem()[Hovered].GetItemInformation()->nGold;

			}
			else if (m_bIsPCItemCilck == true)
			{
				HoveredItemCount = Character->GetInventory()->GetItem()[Hovered].GetItemInformation()->nCount;
				HoveredItemGold = Character->GetInventory()->GetItem()[Hovered].GetItemInformation()->nGold;
			}

			if (HoveredItemCount >= m_CountEditableText_Count)
			{
				m_Count_TotalMoney = HoveredItemGold * m_CountEditableText_Count;
				TotalGoldText->SetText(UKismetTextLibrary::Conv_IntToText(m_Count_TotalMoney));
				m_CountEditableText_CountBuffer = m_CountEditableText_Count;
			}
			else
			{
				m_Count_TotalMoney = HoveredItemGold * HoveredItemCount;
				TotalGoldText->SetText(UKismetTextLibrary::Conv_IntToText(m_Count_TotalMoney));
				CountEditableText->SetText(UKismetTextLibrary::Conv_IntToText(HoveredItemCount));
				m_CountEditableText_CountBuffer = HoveredItemCount;
				// 확인 후 삭제
				//m_Count_TotalMoney = HoveredItemGold * m_CountEditableText_CountBuffer;
				//TotalGoldText->SetText(UKismetTextLibrary::Conv_IntToText(m_Count_TotalMoney));
				//CountEditableText->SetText(UKismetTextLibrary::Conv_IntToText(m_CountEditableText_CountBuffer));
			}
		}
	}

	//APlayerController* PController = Cast<APlayerController>(Character->GetController());
	//PController->SetInputMode(FInputModeGameAndUI());
}

void UOKBoxUserWidget::CountEditTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 사용안함
	return;
	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	APlayerController* PController = Cast<APlayerController>(Character->GetController());
	// 게임 모드로 돌리면 마우스가 움직인다 해결 바람 이러면 마우스클릭이 문제가 된다 좀더 고민해볼것
	PController->SetInputMode(FInputModeGameOnly());
	Character->SetbIsCountVisibility(true);
	Character->SetCountBoxUICameraRotation();
}

void UOKBoxUserWidget::MessageButtonClickedFunction()
{
	MessageBorder->SetVisibility(ESlateVisibility::Hidden);
}