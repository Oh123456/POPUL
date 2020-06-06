// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseCharacter.h"
#include "StorageCharacter.generated.h"

/**
 * 
 */
// 플레이어 창고
UCLASS()
class POPUL_API AStorageCharacter : public AEnemyBaseCharacter
{
	GENERATED_BODY()
	
public:
	AStorageCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMesh;
};
