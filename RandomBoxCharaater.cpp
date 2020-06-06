// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomBoxCharaater.h"
#include "Components/StaticMeshComponent.h"

ARandomBoxCharaater::ARandomBoxCharaater()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->AttachTo((USceneComponent*)GetMesh());
}

void ARandomBoxCharaater::BeginPlay()
{
	bIsActorType = ActorType::E_BOX;
	m_MyInventory.SetMaxInventoryItem(81);
	Super::BeginPlay();
	bIsActorType = ActorType::E_BOX;
	m_bIsNoDamage = true;
	Name = L"상   자";
}