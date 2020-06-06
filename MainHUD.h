// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CTextureLoad.h"
#include "TextureManager.h"
#include "MainHUD.generated.h"

/**
 * 
 */

#define CREATEITMELISTMAX 60
#define ENEMYITEMSLOTMAX 81

UCLASS()
class POPUL_API AMainHUD : public ATextureManager
{
	GENERATED_BODY()
	
public:
	AMainHUD();

	class UUserWidget* __GetDefualtWidget() { return DefualtWidget; }
	UFUNCTION(BlueprintCallable, Category = "MainHUD", meta = (DisplayName = "GetToolTipWidget"))
		class UItemToolTipUserWidget* __GetToolTipWidget() { return ToolTipWidget; }
	class UUserWidget* __GetOkBoxWidget()	{ return OKBoxWidget;}
	class UUserWidget** __GetIemSlotWidget() { return ItemSlotWidget; }
	class UUserWidget** __GetEnemyIemSlotWidget() { return EnemyItemSlotWidget; }
	//테스트용
	UTexture2D* TTTT;
	UTexture2D* TTTT2;

	void CopyITemSlot(UUserWidget** Copy,int Max);
protected:
	virtual void BeginPlay() override;

private:
	class UUserWidget* DefualtWidget;
	class UUserWidget* InventoyWidget;
	//class UUserWidget* ToolTipWidget;
	class UItemToolTipUserWidget* ToolTipWidget;
	class UUserWidget* OKBoxWidget;
	class UUserWidget* ItemSlotWidget[CREATEITMELISTMAX];
	class UUserWidget* EnemyItemSlotWidget[ENEMYITEMSLOTMAX];
};
