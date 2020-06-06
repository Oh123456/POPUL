// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_NewGame = Cast<UButton>(GetWidgetFromName(TEXT("NewGameButton")));
	Button_Continue = Cast<UButton>(GetWidgetFromName(TEXT("ContinueButton")));
	Button_QiutGame = Cast<UButton>(GetWidgetFromName(TEXT("QuitGameButton")));
	
	Button_NewGame->OnClicked.AddDynamic(this,&UMainMenuWidget::NewGameButtonClicked);
	Button_Continue->OnClicked.AddDynamic(this, &UMainMenuWidget::ContinueButtonClicked);
	Button_QiutGame->OnClicked.AddDynamic(this, &UMainMenuWidget::QiutGameButtonClicked);

	//Button_Continue->bIsEnabledDelegate.BindDynamic(this,&UMainMenuWidget::ContinueButtonEnabled);
}

void UMainMenuWidget::NewGameButtonClicked()
{
	UGameplayStatics::DeleteGameInSlot(L"SaveData",0);
	UGameplayStatics::OpenLevel(GetWorld() ,L"ThirdPersonExampleMap");
}

void UMainMenuWidget::ContinueButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), L"HouseMap");
}

bool UMainMenuWidget::ContinueButtonEnabled()
{
	Button_Continue->bIsEnabled = false;
	return UGameplayStatics::DoesSaveGameExist(L"SaveData", 0);
}

void UMainMenuWidget::QiutGameButtonClicked()
{
	UWorld* World = GetWorld();
	UKismetSystemLibrary::QuitGame(World, UGameplayStatics::GetPlayerController(World,0),EQuitPreference::Quit, false);
}