// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CInventory.h"
#include "POPULCharacter.h"


void ANPCBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsActorType = ActorType::E_NPC;
	m_MyInventory.SetMoney(1000);
	m_NPCType = NPCType::E_Shop;
	m_Ability.nHP = -1;
	m_bIsNoDamage = true;
}

void ANPCBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

