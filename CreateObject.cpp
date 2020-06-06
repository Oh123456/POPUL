// Fill out your copyright notice in the Description page of Project Settings.

#include "CreateObject.h"
#include "Components/StaticMeshComponent.h"


void ACreateObject::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComponent->SetSimulatePhysics(false);
	bIsActorType = ActorType::E_CreateObject;
}


void ACreateObject:: Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}