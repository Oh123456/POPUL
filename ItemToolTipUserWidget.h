// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemToolTipUserWidget.generated.h"


/**
 * 
 */
UCLASS()
class POPUL_API UItemToolTipUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetNameTextBlock(class FText Text);
	void SetNameTextBlock(FString String);
	void SetItemTextBlock(class FText Text);
	void SetItemTextBlock(FString String);

	UFUNCTION(BlueprintCallable, Category = "ItemToolTipUserWidget", meta = (DisplayName = "SetItemInformaition"))
		void SetItemInformaition(class UInventorySlotUserWidget* ItemSlotWidget);
	// Static 개행 기능
	static FString Newline(FString String , int Max);
protected:
	virtual void NativeConstruct() override;
private:
	// 개행 기능
	FString Newline(FString String);
	// int 형 아이템 능력을 FString으로 변환
	FString IntItemAbilityInformaition(int Data, FString AbilityName, FString StackString);
	// float 형 아이템 능력을 FString으로 변환
	FString floatItemAbilityInformaition(float Data, FString AbilityName, FString StackString);
private:
	UPROPERTY()
		class UTextBlock* NameTextBlock;
	UPROPERTY()
		class UTextBlock* ItemAbilityTextBlock;
	UPROPERTY()
		class UTextBlock* ItemTextBlock;
	UPROPERTY()
		class UTextBlock* GoldTextBlock;
};
