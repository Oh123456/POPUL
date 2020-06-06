// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageFontActor.generated.h"

UCLASS()
class POPUL_API ADamageFontActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageFontActor();

	// 데미지 폰트 활성화시간 세팅
	void VisibilityTimer(float Time = 1.5f);
	// 데미지 숫자 세팅
	void SetDamageText(int Value);
	class UWidgetComponent* GetDamageTextWidget() const { return DamageText; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	// 데미지 폰트 활성화 시간
	UFUNCTION()
		void VisibilityTimerFunction();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* DamageText;

	FTimerHandle DamageTextTimerHanlde;
};
