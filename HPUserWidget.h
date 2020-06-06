// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPUserWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnHPProgressBarUpdata, ABaseCharacter*, Character);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPProgressBarUpdata, ABaseCharacter*, Character);

#define VISIBILITYTIME 35.0f

UCLASS()
class POPUL_API UHPUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void UpdataHP(class ABaseCharacter* Character);
	void SetVisibilityTimer();
	void SetVisibility(ESlateVisibility Visibility);
protected:
	virtual void NativeConstruct() override;
private:
	void SetVisibility();
public:
	FOnHPProgressBarUpdata HPUpdata;
private:
	UPROPERTY()
		class UProgressBar* HPProgressBar;
	FTimerHandle VisibilityTimerHandle;
};
