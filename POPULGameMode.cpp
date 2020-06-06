// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "POPULGameMode.h"
#include "POPULCharacter.h"
#include "MainHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"
#include "Sound/SoundWave.h"
#include "GameFramework/PlayerStart.h"


#include "POPUL.h"
#include "GameDataInstance.h"
#include "CItem.h"
#include "AStruct.h"
#include "MyAIController.h"
#include "EnemyBaseCharacter.h"
#include "BaseActor.h"
#include "DefualtUserWidget.h"
#include "DamageFontActor.h"
#include "DamageTextUserWidget.h"
#include "DefaultEnemyAIController.h"
#include "ItemDataSaveGame.h"

#include <locale.h>
#include <wchar.h>
//UDefualtUserWidget* APOPULGameMode::DefualtWidget = nullptr;


FName APOPULGameMode::PlayerSpawnStartTag;

APOPULGameMode::APOPULGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/PC/Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	HUDClass = AMainHUD::StaticClass();



	static ConstructorHelpers::FClassFinder<AActor> Tomato(TEXT("/Game/Test/NewBlueprint4"));
	if (Tomato.Class != nullptr)
	{
		TomatoActorClass = Tomato.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> Tomato2(TEXT("/Game/Actor/WoodBlueprint"));
	if (Tomato2.Class != nullptr)
	{
		TomatoActorClass2 = Tomato2.Class;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundWave> DrinkS(TEXT("/Game/Sound/DrinkingSound.DrinkingSound"));
	if (DrinkS.Succeeded())
	{
		DrinkingSound = DrinkS.Object;
	}
	
}

void APOPULGameMode::BeginPlay()
{
	Super::BeginPlay();

	//APOPULGameMode::SpawnActor_Box(TomatoActorClass2,FName(L"HouseMapWoodStart"), FName(L"HouseMapWoodEnd"),10);
	UWorld* World = GetWorld();
	UGameDataInstance* ItemDataInstance = (UGameDataInstance*)UGameplayStatics::GetGameInstance(World);
	ItemDataInstance->SetbIsMoveLevel(false);


	APOPULCharacter* Character = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	TArray<AActor*> arraypActor;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), arraypActor);
	if (!Character->bIsMoveLevel)
		APOPULGameMode::PlayerSpawnLocation(Character, arraypActor,FName(L"HouseMapPlayer"));
	else
		APOPULGameMode::PlayerSpawnLocation(Character, arraypActor, PlayerSpawnStartTag);
	bUseSeamlessTravel = true;
}

CItem APOPULGameMode::GetItemData(int32 ItemCode)
{
	_wsetlocale(LC_ALL, L"korean");
	UGameDataInstance* ItemDataInstance = (UGameDataInstance*)UGameplayStatics::GetGameInstance((UObject*)GetWorld());
	UDataTable* ITemDataTable = ItemDataInstance->GetItemDataTable();
	FString ItemCodeString = FString::FromInt(ItemCode);
	FItemData* ItemBuffer = ITemDataTable->FindRow<FItemData>(UKismetStringLibrary::Conv_StringToName(FString::FromInt(ItemCode)), L"");
	if (ItemBuffer == nullptr)
	{
		CItem ItemData;
		return ItemData;
	}
	else
	{
		CItem ItemData(*ItemBuffer);
		return ItemData;
	}
}

bool APOPULGameMode::GameDataNullCheck(int32 ItemCode)
{
	UGameDataInstance* ItemDataInstance = (UGameDataInstance*)UGameplayStatics::GetGameInstance((UObject*)GetWorld());
	UDataTable* ITemDataTable = ItemDataInstance->GetItemDataTable();
	FString ItemCodeString = FString::FromInt(ItemCode);
	FItemData* ItemBuffer = ITemDataTable->FindRow<FItemData>(UKismetStringLibrary::Conv_StringToName(FString::FromInt(ItemCode)), L"");
	if (ItemBuffer == nullptr)
		return true;
	return false;
}


FString* APOPULGameMode::GetDialog(FString Row)
{
	UGameDataInstance* ItemDataInstance = (UGameDataInstance*)UGameplayStatics::GetGameInstance((UObject*)GetWorld());
	UDataTable* DialogDataTable = ItemDataInstance->GetDialogTable();
	FDialogData* DialogData = DialogDataTable->FindRow<FDialogData>(UKismetStringLibrary::Conv_StringToName(Row), L"");
	if (DialogData == nullptr)
		return nullptr;
	if (!DialogData->Name.IsEmpty())
		Dialog[0] = DialogData->Name;
	if (!DialogData->DialogText1.IsEmpty())
		Dialog[1] = DialogData->DialogText1;
	if (!DialogData->DialogText2.IsEmpty())
		Dialog[2] = DialogData->DialogText2;
	else
		Dialog[2] = L"";
	if (!DialogData->DialogText3.IsEmpty())
		Dialog[3] = DialogData->DialogText3;
	else
		Dialog[3] = L"";
	if (!DialogData->DialogText4.IsEmpty())
		Dialog[4] = DialogData->DialogText4;
	else
		Dialog[4] = L"";
	if (!DialogData->DialogText5.IsEmpty())
		Dialog[5] = DialogData->DialogText5;
	else
		Dialog[5] = L"";
	if (!DialogData->DialogText6.IsEmpty())
		Dialog[6] = DialogData->DialogText6;
	else
		Dialog[6] = L"";

	return Dialog;
}

template <typename T>
bool APOPULGameMode::bIsZeroCheck(T Value)
{
	if (Value == (T)0)
		return true;
	return false;
}


void APOPULGameMode::UsingItem(CItem* Item, AActor* Actor)
{
	ItemStruct* ItemData = Item->GetItemInformation();
	bool bIsMaxHP = false;
	if (ItemData == nullptr)
		return;
	if (ItemData->nCount <= 0)
		return;
	APOPULCharacter* Character = Cast<APOPULCharacter>(Actor);
	if (Character != nullptr)
	{
		if (Item->GetItemInformation()->Itemtype == Itemtype::E_CONSUMPTION)
		{
			Ability* Ab = Character->GetOriginAbility();
			if (!APOPULGameMode::bIsZeroCheck(ItemData->ItemAbility.nMaxHP))
			{
				Ab->nMaxHP = Ab->nMaxHP + ItemData->ItemAbility.nMaxHP;
				Character->UpDataAbility();
				bIsMaxHP = false;
			}
			if (!APOPULGameMode::bIsZeroCheck(ItemData->ItemAbility.nHP))
			{
				if (Character->IsMaxHP() == true)
				{
					Character->AddHP(0);
					bIsMaxHP = true;
				}
				else
				{
					Character->AddHP(ItemData->ItemAbility.nHP);
					bIsMaxHP = false;
				}
			}
			if (!APOPULGameMode::bIsZeroCheck(ItemData->ItemAbility.nSTR))
			{
				Ab->nSTR += ItemData->ItemAbility.nSTR;
				bIsMaxHP = false;
			}
			if (!APOPULGameMode::bIsZeroCheck(ItemData->ItemAbility.nDEX))
			{
				Ab->nDEX += ItemData->ItemAbility.nDEX;
				bIsMaxHP = false;
			}
			if (!APOPULGameMode::bIsZeroCheck(ItemData->ItemAbility.nAttack))
			{
				Ab->nAttack += ItemData->ItemAbility.nAttack;
				bIsMaxHP = false;
			}
			if (!APOPULGameMode::bIsZeroCheck(ItemData->ItemAbility.nDefend))
			{
				Ab->nDefend += ItemData->ItemAbility.nDefend;
				bIsMaxHP = false;
			}
			if (!APOPULGameMode::bIsZeroCheck(ItemData->ItemAbility.nAttackSpeed))
			{
				Ab->nAttackSpeed += ItemData->ItemAbility.nAttackSpeed;
				bIsMaxHP = false;
			}
			if (!APOPULGameMode::bIsZeroCheck(ItemData->ItemAbility.nMoveSpeed))
			{
				Ab->nMoveSpeed += ItemData->ItemAbility.nMoveSpeed;
				bIsMaxHP = false;
			}
			if (bIsMaxHP == true)
				return;
		}
		ItemData->nCount -= 1;
		Character->UpDataAbility();
		return;
	}
}

void APOPULGameMode::UsingItem_Ex(CItem* Item, AActor* Actor)
{
	if (Item == nullptr)
		return;
	if (DrinkingSound != nullptr && Item->GetItemInformation()->Itemtype == Itemtype::E_CONSUMPTION)
		UGameplayStatics::PlaySound2D(GetWorld(), DrinkingSound);
	APOPULGameMode::UsingItem(Item, Actor);
	if (Item->GetItemInformation()->nCount <= 0)
	{
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Actor);
		if (BaseCharacter != nullptr)
			BaseCharacter->GetInventory()->CleanItemData();
	}
	APlayerController* PCController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AMainHUD* HUD = Cast<AMainHUD>(PCController->GetHUD());
	UDefaultUserWidget* Widget = Cast<UDefaultUserWidget>(HUD->__GetDefualtWidget());
	Widget->UpDataWidget();
}

void APOPULGameMode::OpenLevel(FString LevelName)
{
	FName fNameLevelName(UKismetStringLibrary::Conv_StringToName(LevelName));
	APOPULGameMode::OpenLevel(fNameLevelName);
}

void APOPULGameMode::OpenLevel(FName LevelName)
{
	APOPULCharacter* Character = Cast< APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UGameDataInstance* Data = (UGameDataInstance*)(UGameplayStatics::GetGameInstance(GetWorld()));
	Data->SetbIsMoveLevel(true);
	Data->SavePCDataOpenLevel(Character);
	Character->bIsMoveLevel = true;
	Character->GetInventory()->Delete();
	//FName LevelName(L"HouseInMap");
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	//Data->LoadPCDataOpenLevel(Character);
}

void APOPULGameMode::QuitGame()
{
	APOPULGameMode::SaveGameData();
	UKismetSystemLibrary::QuitGame((UObject*)GetWorld(),UGameplayStatics::GetPlayerController((UObject*)GetWorld(),0), EQuitPreference::Quit,true);
	APOPULCharacter* Character = Cast< APOPULCharacter>(UGameplayStatics::GetPlayerCharacter((UObject*)GetWorld(), 0));
	Character->bIsMoveLevel = false;

}

void APOPULGameMode::SaveGameData()
{
	APOPULCharacter* PC = Cast<APOPULCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	UItemDataSaveGame* Save = Cast<UItemDataSaveGame>(UGameplayStatics::CreateSaveGameObject(UItemDataSaveGame::StaticClass()));
	int i = 0;
	int AddItemCount = 0;
	CItem* PCItem;
	CInventory* PCInventory = PC->GetInventory();
	PCItem = PCInventory->GetItem();
	for (i = 0; i < PCInventory->GetMax(); i++)
	{
		if (PC->GetInventory()->GetItem() != nullptr)
		{
			if (PC->GetInventory()->GetItem()[i].GetItemInformation()->nItemID != 0)
				Save->SaveItem(&PCItem[i],i);
		}
	}
	CEquipment* PCEquipment = PC->GetEquipmet();
	UItem* EBufferW = NewObject<UItem>(UItem::StaticClass());;
	PCItem = PCEquipment->GetEquipmentItem(EquipmentItemKinds::E_Weapone);
	if (PCItem->GetItemInformation()->nItemID != 0)
		Save->SaveItem(PCItem, EquipmentItemKinds::E_Weapone,SaveItemType::E_Equipment);
	UItem* EBufferA = NewObject<UItem>(UItem::StaticClass());;
	PCItem = PCEquipment->GetEquipmentItem(EquipmentItemKinds::E_Armor);
	if (PCItem->GetItemInformation()->nItemID != 0)
		Save->SaveItem(PCItem, EquipmentItemKinds::E_Armor, SaveItemType::E_Equipment);

	Save->PCAbility = *PC->GetAbility();
	Save->PCOriginAbility = *PC->GetOriginAbility();
	Save->Money = PC->GetInventory()->GetMoney();
	Save->TestNaem = L"ASDFASDF";
	UGameplayStatics::SaveGameToSlot(Save , Save->SaveSlotName, Save->UserIndex);
}

void APOPULGameMode::PlayerSpawnLocation(ACharacter* Player, TArray<AActor*> PlayerStart,FName PlayerStartTag)
{
	int i = 0;
	bool bIsNoPlayerStart = true;
	for (i = 0; i < PlayerStart.Num() ; i++)
	{
		AActor* Actor = PlayerStart.GetData()[i];
		{
			APlayerStart* PS = Cast<APlayerStart>(Actor);
			if (PS == nullptr)
				continue;
			if (UKismetMathLibrary::EqualEqual_NameName(PlayerStartTag, PS->PlayerStartTag))
			{
				Player->SetActorLocation(PS->GetActorLocation());
				Player->SetActorRotation(PS->GetActorRotation());
				bIsNoPlayerStart = false;
			}
		}
	}

	if (bIsNoPlayerStart == true)
	{
		for (i = 0; i < PlayerStart.Num(); i++)
		{
			AActor* Actor = PlayerStart.GetData()[i];
			{
				APlayerStart* PS = Cast<APlayerStart>(Actor);
				if (PS == nullptr)
					continue;
				if (UKismetMathLibrary::EqualEqual_NameName(L"HouseMapPlayer", PS->PlayerStartTag))
				{
					Player->SetActorLocation(PS->GetActorLocation());
					Player->SetActorRotation(PS->GetActorRotation());
					bIsNoPlayerStart = false;
				}
			}
		}
	}
}

void APOPULGameMode::SpawnActor_Location(TSubclassOf<AActor> Actor, TArray<AActor*> PlayerStart, FName PlayerStartTag, int SpawnCount ,float XRange , float YRange , float ZRange )
{
	int i = 0, j = 0;
	FName tag;
	UWorld* World = GetWorld();
	FVector Location;
	FRotator Rotator(0.f, 0.f, 0.f);
	FActorSpawnParameters Pa;
	Pa.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	
	for (i = 0; i < PlayerStart.Num(); i++)
	{
		APlayerStart* PS = Cast<APlayerStart>(PlayerStart.GetData()[i]);
		if (PS == nullptr)
			continue;
		if (UKismetMathLibrary::EqualEqual_NameName(PlayerStartTag, PS->PlayerStartTag))
		{
			Location = PS->GetActorLocation();
			for (j = 0; j < SpawnCount; j++)
			{
				if (XRange != 0)
				{
					float XRangeRand = rand() % (int)(XRange * 100);
					XRangeRand = XRangeRand / 100;
					Location.X += XRangeRand - XRange / 2;
				}
				if (YRange != 0)
				{
					float YRangeRand = rand() % (int)(YRange * 100);
					YRangeRand = YRangeRand / 100;
					Location.Y += YRangeRand - XRange / 2;
				}
				if (ZRange != 0)
				{
					float ZRangeRand = rand() % (int)(ZRange * 100);
					ZRangeRand = ZRangeRand / 100;
					Location.Z += ZRangeRand - XRange / 2;
				}
				World->SpawnActor<AActor>(Actor, Location, FRotator(0, 0, 0), Pa);
			}
		}
	}

}


void APOPULGameMode::SpawnActor_Box(TSubclassOf<AActor> Actor, TArray<AActor*> PlayerStart, FName PlayerStart_StartTag, FName PlayerStart_EndTag, int SpawnCount)
{
	int i = 0, j = 0;
	int Count = 0;
	FName tag;
	UWorld* World = GetWorld();
	FVector StartLocation;
	FVector EndLocation;
	FRotator Rotator(0.f, 0.f, 0.f);
	FActorSpawnParameters Pa;
	Pa.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	
	for (i = 0; i < PlayerStart.Num(); i++)
	{
		if (Count == 2)
			break;
		APlayerStart* PS = Cast<APlayerStart>(PlayerStart.GetData()[i]);
		if (PS == nullptr)
			continue;
		if (UKismetMathLibrary::EqualEqual_NameName(PlayerStart_StartTag, PS->PlayerStartTag))
		{
			StartLocation = PS->GetActorLocation();
			Count++;
			continue;
		}
		if (UKismetMathLibrary::EqualEqual_NameName(PlayerStart_EndTag, PS->PlayerStartTag))
		{
			EndLocation = PS->GetActorLocation();
			Count++;
			continue;
		}
	}

	if (Count != 2)
		return;
	FVector Location;
	for (i = 0; i < SpawnCount; i++)
	{
		int X = (StartLocation.X - EndLocation.X) * 100;
		X = abs(X);
		float fX = rand() % X;
		fX = fX / 100;
		int Y = (StartLocation.Y - EndLocation.Y) * 100;
		Y = abs(Y);
		float fY = rand() % Y;
		fY = fY / 100;

		Location.X = StartLocation.X + fX;
		Location.Y = StartLocation.Y + fY;
		Location.Z = StartLocation.Z >= StartLocation.Z ? StartLocation.Z : EndLocation.Z;

		World->SpawnActor<AActor>(Actor, Location, FRotator(0, 0, 0), Pa);
	}
}

void APOPULGameMode::SpawnActor_Box_Rand(TSubclassOf<AActor> Actor, TArray<AActor*> PlayerStart, FName PlayerStart_StartTag, FName PlayerStart_EndTag,bool Reversal ,int SpawnCount )
{
	int i = 0, j = 0;
	int Count = 0;
	FName tag;
	UWorld* World = GetWorld();
	FVector StartLocation;
	FVector EndLocation;
	FRotator Rotator(0.f, 0.f, 0.f);
	FActorSpawnParameters Pa;
	Pa.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	for (i = 0; i < PlayerStart.Num(); i++)
	{
		if (Count == 2)
			break;
		APlayerStart* PS = Cast<APlayerStart>(PlayerStart.GetData()[i]);
		if (PS == nullptr)
			continue;
		if (UKismetMathLibrary::EqualEqual_NameName(PlayerStart_StartTag, PS->PlayerStartTag))
		{
			StartLocation = PS->GetActorLocation();
			Count++;
			continue;
		}
		if (UKismetMathLibrary::EqualEqual_NameName(PlayerStart_EndTag, PS->PlayerStartTag))
		{
			EndLocation = PS->GetActorLocation();
			Count++;
			continue;
		}
	}

	if (Count != 2)
		return;
	FVector Location(0, 0, 0);
	FVector SpawnLocation(0, 0, 0);
	bool Ismaenes = false;
	float Xw = StartLocation.X - EndLocation.X;
	float Yw = StartLocation.Y - EndLocation.Y;
	ABaseActor* BaseActor = Cast<ABaseActor>(World->SpawnActor<AActor>(Actor, Location, FRotator(0, 0, 0), Pa));
	UBoxComponent* Box = BaseActor->BoxComponent;
	FVector BoxExtent = Box->GetScaledBoxExtent();
	TArray<AActor*> arryActor;
	TArray<FHitResult> arryHit;
	FHitResult Hit;
	int failureCount = 0;
	bool bIsSpawn = true;

	for (i = 0; i < SpawnCount; i++)
	{
		int X = rand() % (int)Xw;
		int Y = rand() % (int)Yw;

		if (!Reversal)
		{
			SpawnLocation.X = StartLocation.X + X;
			SpawnLocation.Y = StartLocation.Y + Y;
			SpawnLocation.Z = StartLocation.Z;
		}
		else
		{
			SpawnLocation.X = StartLocation.X - X;
			SpawnLocation.Y = StartLocation.Y - Y;
			SpawnLocation.Z = StartLocation.Z;
		}

		bool IsOk = UKismetSystemLibrary::BoxTraceMulti(World, SpawnLocation, SpawnLocation, BoxExtent, FRotator(0.0f, 0.0f, 0.0f), ETraceTypeQuery::TraceTypeQuery1, false, arryActor, EDrawDebugTrace::Type::None, arryHit, false);
		bIsSpawn = true;
		if (IsOk)
		{
			for (int j = 0; j < arryHit.Num(); j++)
			{
				ABaseActor* BActor = Cast<ABaseActor>(arryHit.GetData()[j].GetActor());
				if (BActor == nullptr)
					continue;
				else
				{
					failureCount++;
					if (failureCount > 10)
					{
						failureCount = 0;
						bIsSpawn = false;
					}
					break;
				}

			}
			if (failureCount != 0)
			{
				i--;
				continue;
			}
		}
		if (bIsSpawn == true)
			World->SpawnActor<AActor>(Actor, SpawnLocation, FRotator(0, 0, 0));

	}
}

void APOPULGameMode::SpawnCharacter_Box_Rand(TSubclassOf<ACharacter> Actor, TArray<AActor*> PlayerStart, FName PlayerStart_StartTag, FName PlayerStart_EndTag, bool Reversal , int SpawnCount)
{
	int i = 0, j = 0;
	int Count = 0;
	FName tag;
	UWorld* World = GetWorld();
	FVector StartLocation;
	FVector EndLocation;
	FRotator Rotator(0.f, 0.f, 0.f);
	FActorSpawnParameters Pa;
	Pa.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	for (i = 0; i < PlayerStart.Num(); i++)
	{
		if (Count == 2)
			break;
		APlayerStart* PS = Cast<APlayerStart>(PlayerStart.GetData()[i]);
		GEngine->AddOnScreenDebugMessage(1,1.0f,FColor::Black , PS->PlayerStartTag.ToString());
		if (PS == nullptr)
			continue;
		if (UKismetMathLibrary::EqualEqual_NameName(PlayerStart_StartTag, PS->PlayerStartTag))
		{
			StartLocation = PS->GetActorLocation();
			Count++;
			continue;
		}
		if (UKismetMathLibrary::EqualEqual_NameName(PlayerStart_EndTag, PS->PlayerStartTag))
		{
			EndLocation = PS->GetActorLocation();
			Count++;
			continue;
		}
	}

	if (Count != 2)
	{
		UE_LOG(POPUL,Warning,TEXT("SpawnLocation Not Two"))
		return;
	}
	FVector Location(0, 0, 0);
	FVector SpawnLocation(0, 0, 0);
	bool Ismaenes = false;
	float Xw = StartLocation.X - EndLocation.X;
	float Yw = StartLocation.Y - EndLocation.Y;
	AEnemyBaseCharacter* BaseActor = Cast<AEnemyBaseCharacter>(World->SpawnActor<AActor>(Actor, Location, FRotator(0, 0, 0), Pa));
	UBoxComponent* Box = BaseActor->BoxComponent;
	FVector BoxExtent = Box->GetScaledBoxExtent();
	TArray<AActor*> arryActor;
	TArray<FHitResult> arryHit;
	FHitResult Hit;
	int failureCount = 0;
	bool bIsSpawn = true;

	for (i = 0; i < SpawnCount; i++)
	{
		int X = rand() % (int)Xw;
		int Y = rand() % (int)Yw;

		if (!Reversal)
		{
			SpawnLocation.X = StartLocation.X + X;
			SpawnLocation.Y = StartLocation.Y + Y;
			SpawnLocation.Z = StartLocation.Z;
		}
		else
		{
			SpawnLocation.X = StartLocation.X - X;
			SpawnLocation.Y = StartLocation.Y - Y;
			SpawnLocation.Z = StartLocation.Z;
		}

		bool IsOk = UKismetSystemLibrary::BoxTraceMulti(World, SpawnLocation, SpawnLocation, BoxExtent, FRotator(0.0f, 0.0f, 0.0f), ETraceTypeQuery::TraceTypeQuery1, false, arryActor, EDrawDebugTrace::Type::None, arryHit, false);
		bIsSpawn = true;
		if (IsOk)
		{
			for (int j = 0; j < arryHit.Num(); j++)
			{
				AEnemyBaseCharacter* BActor = Cast<AEnemyBaseCharacter>(arryHit.GetData()[j].GetActor());
				if (BActor == nullptr)
					continue;
				else
				{
					failureCount++;
					if (failureCount > 10)
					{
						failureCount = 0;
						bIsSpawn = false;
					}
					break;
				}

			}
			if (failureCount != 0)
			{
				i--;
				continue;
			}
		}
		if (bIsSpawn == true)
		{
			AEnemyBaseCharacter* EnemySpawnActor = World->SpawnActor<AEnemyBaseCharacter>(Actor, SpawnLocation, FRotator(0, 0, 0));
			if (EnemySpawnActor != nullptr)
			{
				EnemySpawnActor->AIControllerClass = ADefaultEnemyAIController::StaticClass();
			}
		}
	}
}

void APOPULGameMode::SpawnActor_UniformityBox(TSubclassOf<AActor> Actor, TArray<AActor*> PlayerStart, FName PlayerStart_StartTag, FName PlayerStart_EndTag, int SpawnCount)
{
	int i = 0, j = 0;
	int Count = 0;
	FName tag;
	UWorld* World = GetWorld();
	FVector StartLocation;
	FVector EndLocation;
	FRotator Rotator(0.f, 0.f, 0.f);
	FActorSpawnParameters Pa;
	Pa.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	for (i = 0; i < PlayerStart.Num(); i++)
	{
		if (Count == 2)
			break;
		APlayerStart* PS = Cast<APlayerStart>(PlayerStart.GetData()[i]);
		if (PS == nullptr)
			continue;
		if (UKismetMathLibrary::EqualEqual_NameName(PlayerStart_StartTag, PS->PlayerStartTag))
		{
			StartLocation = PS->GetActorLocation();
			Count++;
			continue;
		}
		if (UKismetMathLibrary::EqualEqual_NameName(PlayerStart_EndTag, PS->PlayerStartTag))
		{
			EndLocation = PS->GetActorLocation();
			Count++;
			continue;
		}
	}

	if (Count != 2)
		return;
	FVector Location(0,0,0);
	FVector SpawnLocation(0, 0, 0);
	ABaseActor* BaseActor = Cast<ABaseActor>(World->SpawnActor<AActor>(Actor, Location, FRotator(0, 0, 0), Pa));
	UBoxComponent* Box = BaseActor->BoxComponent;
	FVector BoxExtent = Box->GetScaledBoxExtent();
	bool Ismaenes = false;
	float Xw = StartLocation.X - EndLocation.X;
	float Yw = StartLocation.Y - EndLocation.Y;
	if (Xw < 0)
	{
		Ismaenes = true;
		Xw *= -1;
		Yw *= -1;
	}
	float XCount = 0.0f;
	float YCount = 0.0f;
	for (i = 0; i < SpawnCount; i++)
	{
		Location = StartLocation;
		if (XCount <= Xw + BoxExtent.X / 2)
		{
			if (!Ismaenes)
			{
				SpawnLocation.X = Location.X + XCount;
				SpawnLocation.Y = Location.Y + YCount;
				SpawnLocation.Z = Location.Z;
			}
			else
			{
				SpawnLocation.X = Location.X + XCount;
				SpawnLocation.Y = Location.Y + YCount;
				SpawnLocation.Z = Location.Z;
			}
		}
		else
		{
			XCount = 0.0f;
			YCount += (BoxExtent.Y);
			if (YCount >= Yw + BoxExtent.Y)
			{
				BaseActor->Destroy();
				return;
			}
			SpawnLocation = Location;
			SpawnLocation.Y = Location.Y + YCount;
		}
		World->SpawnActor<AActor>(Actor, SpawnLocation, FRotator(0, 0, 0));
		XCount += (BoxExtent.X );
	}
	BaseActor->Destroy();

}

void APOPULGameMode::SpawnDamageTextActor(FVector Vector, int Damage)
{
	ADamageFontActor* DamaText = GetWorld()->SpawnActor<ADamageFontActor>(Vector, FRotator(0, 0, 0));
	
	if (DamaText != nullptr)
	{

		UDamageTextUserWidget* DamageTextWidget = Cast<UDamageTextUserWidget>(DamaText->DamageText->GetUserWidgetObject());
		DamageTextWidget->SetDamageText(Damage);
		//DamageTextWidget->SetVisibility_Visible();
		//DamaText->SetDamageText(Damage);
	}

	else
	{
		//GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Blue, TEXT("스폰실패"));
		//UE_LOG(POPUL, Warning, TEXT("스폰실패"));
	}
}