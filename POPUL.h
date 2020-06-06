// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(POPUL, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(")) + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(POPUL, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(POPUL, Verbosity, TEXT("%s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS))