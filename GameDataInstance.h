// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "CEquipment.h"
#include "CInventory.h"
#include "AStruct.h"
#include "ItemDataSaveGame.h"
#include "GameDataInstance.generated.h"

/**
 * 
 */
// 아이템 이름
UENUM(BlueprintType)
enum class EItemName : uint8
{
	E_NONE			= 0U,
	E_WOOD			= 1U,
	E_TOMATO		= 2U,
	E_GOODTOMATO	= 3U,
	E_PLYWOOD		= 4U,
	E_WOODBOX		= 5U,
	E_MYEONGTAE		= 6U,
	E_MEDICINALHERBS = 7U,
	E_WATER			 = 8U,
	E_BOTTLE		 = 9U,
	E_HPPOTIN		 = 10U,
	E_TOMATOJUICE	 = 11U,
	E_TOMATOPOTION	 = 12U,
	E_BIGHPPOTION	 = 13U,
	E_NAIL			 = 14U,
	E_DESK			 = 15U,
	E_CLOSET		 = 16U,
	E_TOMATOBOX		 = 17U,
	E_NORMALSWORD	 = 18U,
	E_NORMALARMOR	 = 19U,
	E_IRONSWORD		 = 20U,
	E_IRON			 = 21U,
	E_CHARBON		 = 22U,
	E_CHARBONSTEEL	 = 23U,
	E_GODMATEL		 = 24U,
	E_LONGSWORD		 = 25U,
	E_HEAVYSWORD	 = 26U,
	E_LEATHER		 = 27U,
	E_THONG			 = 28U,
	E_LIGHTSWORD	 = 29U,
	E_CHARBONSWORD	 = 30U,
	E_BRACKSWORD	 = 31U,
	E_NIGHTSKYSWORD	 = 32U,
	E_BLUESWORD	     = 33U,
	E_DAMASCUSSWORD  = 34U,
	E_GODSWORD		 = 35U,
	E_TEXTSWORD		 = 36U,
	E_REINFORCEARMOR = 37U,
	E_LEATHERARMOR	 = 38U,
	E_RELEATHERARMOR = 39U,
	E_LIGHTARMOR	 = 40U,
	E_RELIGHTARMOR	 = 41U,
	// 장인의 M
	E_MRLIGHTARMOR	 = 42U,
	E_IORNARMOR		 = 43U,
	E_REIORNARMOR	 = 44U,
	E_CHARBONARMOR	 = 45U,
	E_MIORNARMOR	 = 46U,
	E_GODARMOR		 = 47U,
	E_TESTARMOR		 = 48U,
	E_ALLPOTON		 = 49U,
	E_ATTACKPOTON    = 50U,
	E_DEFENDPOTON	 = 51U,
	E_DEXPOTON		 = 52U,
	E_STRPOTON		 = 53U,
	E_CHICKEN		 = 54U,
	E_LICORICE		 = 55U,
	E_ITEMNAMEMAX
};

// 아이템 타입
enum Itemtype
{
	E_ERROR					= 0,
	E_EQUIPMENT				= 1,
	E_CONSUMPTION			= 2,
	E_OTHER					= 3,
	E_EQUIPMENT_WEAPONE		= 4,
	E_EQUIPMENT_ARMOR		= 5,
};


 
UENUM(BlueprintType)
enum class EItemtypes : uint8
{
	E_Error					= 0,
	E_Equipment				= 1,
	E_Consumption			= 2,
	E_Other					= 3,
	E_Equipment_Weapone		= 4,
	E_Equipment_Armor		= 5,
};


UENUM(BlueprintType)
enum class EDailogName : uint8
{
	E_Default			= 0,
	E_MedicineShop		= 1,
	E_AllShop			= 2,
	E_WeaponeShop		= 3,
	E_Merchant			= 4,
	E_DailogMax
};

//enum DailogName
//{
//	E_Default		= 0,
//	E_MedicineShop	= 1,
//	E_AllShop		= 2,
//	E_DailogMax
//};

static FString strDailogName[(int)EDailogName::E_DailogMax]
{
	L"기본문",
	L"약제판매원",
	L"잡상인",
	L"장비판매원",
	L"생황아이템",
};


// 아이템 구조체
USTRUCT(BlueprintType)
struct FItemData :public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemData() {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nImageCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FString fsName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nDefend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		float nMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		float nAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nSTR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nDEX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nItemtype;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialItemID1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialCount1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialItemID2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialCount2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialItemID3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialCount3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialItemID4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialCount4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialItemID5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialCount5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialItemID6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nmaterialCount6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nReturnCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int nCreateUsingHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		bool bIsCreate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		FString fsItemText;
};

//대사 구조체
USTRUCT(BlueprintType)
struct FDialogData :public FTableRowBase
{
	GENERATED_BODY()
public:
	FDialogData() {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDialogData")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDialogData")
		FString DialogText1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDialogData")
		FString DialogText2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDialogData")
		FString DialogText3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDialogData")
		FString DialogText4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDialogData")
		FString DialogText5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDialogData")
		FString DialogText6;
};

UCLASS()
class POPUL_API UGameDataInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UGameDataInstance();

	// 로드 게임데이터
	void LoadGameData();
	// 로드 게임베이터 채크
	void LoadGameDataCheack();
	void LoadDataToTrue() { LoadData = true; }
	void LoadDataToFalse() { LoadData = false; }

	UDataTable* GetItemDataTable() { return ItemData; }
	UDataTable* GetDialogTable() { return DialogData; }
	FItemData* GetItemData(int ItemCode);
	UItemDataSaveGame* GetSaveData() const { return SaveData; }
	bool GetLoadData() const { return LoadData; }

	static bool bIsMoveLevel;
private:
	//UPROPERTY()
	// 아이템 데이터
	static UDataTable* ItemData;
	static UDataTable* DialogData;

public:
	UFUNCTION(BlueprintCallable, Category = "GameDataInstance", meta = (DisplayName = "SavePCDataOpenLevel"))
		void SavePCDataOpenLevel(class APOPULCharacter* PC);
	UFUNCTION(BlueprintCallable, Category = "GameDataInstance", meta = (DisplayName = "LoadPCDataOpenLevel"))
		void LoadPCDataOpenLevel(class APOPULCharacter* PC);

	UFUNCTION(BlueprintCallable, Category = "GameDataInstance", meta = (DisplayName = "SetbIsMoveLevel"))
		void SetbIsMoveLevel(bool BOOL) { bIsMoveLevel = BOOL; }
	UFUNCTION(BlueprintCallable, Category = "GameDataInstance", meta = (DisplayName = "GetbIsMoveLevel"))
		bool GetbIsMoveLevel() {return bIsMoveLevel;}
private:
	// 플레이어 저장 변수
	FAbility PCAbility;
	FAbility PCOriginAbility;
	CInventory PCInventory;
	CInventory* StorageInventory;
	CEquipment PCEquipment;

	// 세이브 데이터
	UItemDataSaveGame* SaveData;

	bool PCIsDeath;
	bool LoadData;
	bool m_bIsPCBattel;
};
