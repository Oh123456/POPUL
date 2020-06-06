// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextUserWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageText, int, Damage);

UCLASS()
class POPUL_API UDamageTextUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	inline void SetVisibility_Visible();
	inline void SetVisibility_Hidden();
	void SetDamageText(int Damage);

	class UTextBlock* GetDamageText() const { return DamageText; }
protected:
	virtual void NativeConstruct() override;
public:
	FOnDamageText DamageTextDelegate;
	int Damage;
private:
	UPROPERTY()
		class UTextBlock* DamageText;
};
