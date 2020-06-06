// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AStruct.generated.h"
// 구조체만사용한다
UCLASS()
class POPUL_API AAStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// 구조체

USTRUCT(BlueprintType)
struct FAbility
{
	GENERATED_BODY()


public:
	FAbility() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		int nMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		int nHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		int nAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		int nDefend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float nMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float nAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		int nSTR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		int nDEX;

};

typedef struct FAbility Ability;

USTRUCT(BlueprintType)
struct FCreateData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialItemID1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialCount1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialItemID2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialCount2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialItemID3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialCount3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialItemID4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialCount4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialItemID5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialCount5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialItemID6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nmaterialCount6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nReturnCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		int nCreateUsingHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateData")
		bool bIsCreate;

};

typedef struct FCreateData CreateData;

USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_BODY()

public:
	FItemStruct() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		int nItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		FAbility ItemAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		int nCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		int Itemtype;
	// 인벤토리 할당된곳
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		int InventoryNumber;
	// 돈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		int nGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		FCreateData Create;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStruct")
		class UInventorySlotUserWidget* ItemWidget;


};

typedef struct FItemStruct ItemStruct;

template <typename T>
bool __IsZero(T Value)
{
	if (Value == (T)0)
		return true;
	else
		return false;
	return false;
}