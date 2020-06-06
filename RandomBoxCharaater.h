// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseCharacter.h"
#include "RandomBoxCharaater.generated.h"

/**
 * 
 */
UCLASS()
class POPUL_API ARandomBoxCharaater : public AEnemyBaseCharacter
{
	GENERATED_BODY()
	
public:
	ARandomBoxCharaater();
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;
};
