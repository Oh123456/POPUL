// Fill out your copyright notice in the Description page of Project Settings.

#include "StorageCharacter.h"
#include "Components/StaticMeshComponent.h"

AStorageCharacter::AStorageCharacter()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->AttachTo((USceneComponent*)GetMesh());
}

void AStorageCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_bIsNoDamage = true;
	bIsActorType = ActorType::E_BOX;
	Name = TEXT("창   고");
}