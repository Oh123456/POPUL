// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActortypeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActortypeInterface : public UInterface
{
	GENERATED_BODY()
};

// 액터 타입
enum ActorType
{
	E_NONE					= -1,
	E_CreateObject			= 0,
	E_Enemy					= 1,
	E_PC					= 2,
	E_NPC					= 3,
	E_DestructionObject		= 4,
	E_NonDestructionObject	= 5,
	E_BOX					= 6,


};
/**
 * 
 */
class POPUL_API IActortypeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	int GetbIsActorType() { return bIsActorType; }
protected:
	int bIsActorType;
};
