// Fill out your copyright notice in the Description page of Project Settings.

#include "NonDestructionObject.h"

void ANonDestructionObject::BeginPlay()
{
	Super::BeginPlay();
	bIsActorType = ActorType::E_NonDestructionObject;
}
void ANonDestructionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}