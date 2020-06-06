// Fill out your copyright notice in the Description page of Project Settings.

#include "AStruct.h"

// Sets default values
AAStruct::AAStruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAStruct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAStruct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

