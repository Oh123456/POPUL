// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInventory.h"
#include "ActortypeInterface.h"
#include "GameDataInstance.h"
#include "BaseActor.generated.h"

#define DEFUALT_ACTOR_INVENTORYMAX 10 

UCLASS()
class POPUL_API ABaseActor : public AActor, public IActortypeInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* DefualtSceneComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool Destroy(bool bNetForce = false, bool bShouldModifyLevel = true);
	bool GetbIsOverlap() const { return m_bIsOverlap; }

	UFUNCTION(BlueprintCallable, Category = "BaseActor", meta = (DisplayName = "SetItem"))
		void SetItem(EItemName ItemCode, int ItemCount);
	UFUNCTION(BlueprintCallable, Category = "BaseActor", meta = (DisplayName = "SetInventoryMax"))
		void SetInventoryMaxSetting(int Max);
	// 아이템 확률
	UFUNCTION(BlueprintCallable, Category = "BaseActor", meta = (DisplayName = "Percentage"))
		bool PercentageFunction(float ItemPercentage, int Percentage = 100);

private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	CInventory* GetInventory() { return &m_MyInventory; }
protected:
	// 인벤토리
	CInventory m_MyInventory;
private:
	CItem Items;
	// 곂처졌는지
	bool m_bIsOverlap;
};
