// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TextureList.h"
#include "TextureManager.generated.h"

/**
 * 
 */

//2D �ؽ��ĸ� ���⼭ �ε��ؼ� ����Ѵ�.

UCLASS()
class POPUL_API ATextureManager : public AHUD
{
	GENERATED_BODY()
public:
	ATextureManager();

	static class UTexture2D** GetTextrues() { return Textrues; }
private:
	static class UTexture2D* Textrues[TextureList::E_MAX];
};
