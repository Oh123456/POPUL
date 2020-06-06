// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CItem.h"
#include "POPULGameMode.generated.h"

#define DIALOG_MAX 7
UCLASS(minimalapi)
class APOPULGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APOPULGameMode();

	bool GameDataNullCheck(int32 ItemCode);
	CItem GetItemData(int32 ItemCode);
	FString* GetDialog(FString Row);

	// 아이템 사용 
	void UsingItem(CItem* Item, AActor* Actor);
	// 아이템 사용 가급적 이결사용
	void UsingItem_Ex(CItem* Item, AActor* Actor);

	// 게임 종료
	void QuitGame();
	// 게임데이터 세이브
	void SaveGameData();
	template <typename T>
	static bool bIsZeroCheck(T Value);
	//static class UDefualtUserWidget* GetDefualtWidget() { return DefualtWidget; }
	// 데미지 폰트 액터 출력
	void SpawnDamageTextActor(FVector Vector, int Damage);

	// 플레이어 위치세팉
	UFUNCTION(BlueprintCallable, Category = "POPULGameMode", meta = (DisplayName = "PlayerSpawnLocation"))
	void PlayerSpawnLocation(ACharacter* Player, TArray<AActor*> PlayerStart,FName PlayerStartTag);
	//  스타트점으로 스폰 XRange X축 랜덤값 YRange Y축 랜덤값 ZRange Z축 랜덤값 Z축은 사용 안하는것이 좋다 
	UFUNCTION(BlueprintCallable, Category = "POPULGameMode", meta = (DisplayName = "SpawnActoratLocation"))
	void SpawnActor_Location(TSubclassOf<AActor> Actor, TArray<AActor*> PlayerStart, FName PlayerStartTag, int SpawnCount = 1, float XRange = 0.0f, float YRange = 0.0f, float ZRange = 0.0f);
	// 두 스타트점으로 사격영역 랜덤위치 스폰
	UFUNCTION(BlueprintCallable, Category = "POPULGameMode", meta = (DisplayName = "SpawnActoratBox"))
	void SpawnActor_Box(TSubclassOf<AActor> Actor, TArray<AActor*> PlayerStart, FName PlayerStart_StartTag, FName PlayerStart_EndTag, int SpawnCount = 1);
	// 두 스타트점으로 사격영역 랜덤위치 스폰 
	UFUNCTION(BlueprintCallable, Category = "POPULGameMode", meta = (DisplayName = "SpawnActoratBoxRandem"))
	void SpawnActor_Box_Rand(TSubclassOf<AActor> Actor, TArray<AActor*> PlayerStart, FName PlayerStart_StartTag, FName PlayerStart_EndTag, bool Reversal = false,int SpawnCount = 1);
	// 두 스타트점으로 사격영역 랜덤위치 스폰 캐릭터
	UFUNCTION(BlueprintCallable, Category = "POPULGameMode", meta = (DisplayName = "SpawnCharacteratBoxRandem"))
	void SpawnCharacter_Box_Rand(TSubclassOf<ACharacter> Actor, TArray<AActor*> PlayerStart, FName PlayerStart_StartTag, FName PlayerStart_EndTag, bool Reversal = false, int SpawnCount = 1);
	// 두 스타트점으로 사각영역 BoxComponents 사이즈로 균등하게 스폰
	UFUNCTION(BlueprintCallable, Category = "POPULGameMode", meta = (DisplayName = "SpawnActoratUniformityBox"))
	void SpawnActor_UniformityBox(TSubclassOf<AActor> Actor, TArray<AActor*> PlayerStart, FName PlayerStart_StartTag, FName PlayerStart_EndTag, int SpawnCount = 1);

	UFUNCTION(BlueprintCallable, Category = "POPULGameMode", meta = (DisplayName = "OpenLevel"))
	void OpenLevel(FName LevelName);
	void OpenLevel(FString LevelName);

	UFUNCTION(BlueprintCallable, Category = "POPULGameMode", meta = (DisplayName = "MoveSpawnPlayerStartTag"))
	void MoveSpawnPlayerStartTag(FName LevelName) { PlayerSpawnStartTag = LevelName; }

protected:
	virtual void BeginPlay() override;
protected:
	static FName PlayerSpawnStartTag;
private:
	class USoundWave* DrinkingSound;
private:
	CItem Test[5];
	FString Dialog[7];

	// 테스토용
	TSubclassOf<AActor> TomatoActorClass;
	TSubclassOf<AActor> TomatoActorClass2;
};


