// Fill out your copyright notice in the Description page of Project Settings.

#include "MainHUD.h"
#include "UserWidget.h"
#include "InventorySlotUserWidget.h"
#include "UObject/ConstructorHelpers.h"


AMainHUD::AMainHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> DW(TEXT("/Game/Widget/Default/DefaultWidgetBlueprint.DefaultWidgetBlueprint_C"));
	if (DW.Succeeded())
	{
		DefualtWidget = CreateWidget<UUserWidget>(GetWorld(), DW.Class);
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> IW(TEXT("/Game/Widget/Inven/InventoyWidgetBlueprint.InventoyWidgetBlueprint_C"));
	if (IW.Succeeded())
	{
		InventoyWidget = CreateWidget<UUserWidget>(GetWorld(), IW.Class);
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> TTW(TEXT("/Game/Widget/Inven/ToolTipWidgetBlueprint.ToolTipWidgetBlueprint_C"));
	if (TTW.Succeeded())
	{
		ToolTipWidget = CreateWidget<UItemToolTipUserWidget>(GetWorld(), TTW.Class);
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> OKW(TEXT("/Game/Widget/Default/OKBoxWidgetBlueprint.OKBoxWidgetBlueprint_C"));
	if (OKW.Succeeded())
	{
		OKBoxWidget = CreateWidget<UUserWidget>(GetWorld(), OKW.Class);
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> ISW(TEXT("/Game/Widget/Inven/InventorySlotWidgetBlueprint.InventorySlotWidgetBlueprint_C"));
	if (ISW.Succeeded())
	{
		for (int i = 0 ;i < CREATEITMELISTMAX;i++)
			ItemSlotWidget[i] = CreateWidget<UUserWidget>(GetWorld(), ISW.Class);
		for(int i = 0; i < ENEMYITEMSLOTMAX;i++)
			EnemyItemSlotWidget[i] = CreateWidget<UUserWidget>(GetWorld(), ISW.Class);

	}
	//테스트용
	//static ConstructorHelpers::FObjectFinder<UTexture2D> Icon(TEXT("/Game/ItemIcon/Wood.Wood"));
	//if (Icon.Succeeded())
	//{
	//	TTTT = Icon.Object;
	//}
	//static ConstructorHelpers::FObjectFinder<UTexture2D> Icon2(TEXT("/Game/Widget/Inven/tomato_1__1_.tomato_1__1_"));
	//if (Icon2.Succeeded())
	//{
	//	TTTT2 = Icon2.Object;
	//}

}

void AMainHUD::CopyITemSlot(UUserWidget** Copy, int Max)
{
	for (int i = 0; i < Max; i++)
		memcpy(Copy[i], ItemSlotWidget, sizeof(UInventorySlotUserWidget));
}

void AMainHUD::BeginPlay()
{
	if (DefualtWidget != nullptr)
		DefualtWidget->AddToViewport();
	if (OKBoxWidget != nullptr)
		OKBoxWidget->AddToViewport();
	//OKBoxWidget->RemoveFromViewport();
	//if (InventoyWidget != nullptr)
		//InventoyWidget->AddToViewport();
}