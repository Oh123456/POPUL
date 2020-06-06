// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class POPUL_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
		void NewGameButtonClicked();
	UFUNCTION()
		void ContinueButtonClicked();
	UFUNCTION()
		bool ContinueButtonEnabled();
	UFUNCTION()
		void QiutGameButtonClicked();
private:
	UPROPERTY()
		class UButton* Button_NewGame;
	UPROPERTY()
		class UButton* Button_Continue;
	UPROPERTY()
		class UButton* Button_QiutGame;
};
