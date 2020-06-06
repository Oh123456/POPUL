// Fill out your copyright notice in the Description page of Project Settings.

#include "TextureManager.h"
#include "Engine/Texture.h"
#include "ConstructorHelpers.h"

UTexture2D* ATextureManager::Textrues[TextureList::E_MAX];


ATextureManager::ATextureManager()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> ItemIcon[TextureList::E_MAX]
	{
		(L"/Game/ItemIcon/Wood.Wood"),
		(L"/Game/ItemIcon/tomato.tomato"),
		(L"/Game/ItemIcon/Plywood.Plywood"),
		(L"/Game/ItemIcon/WoodBox.WoodBox"),
		(L"/Game/ItemIcon/Myeongtae.Myeongtae"),
		(L"/Game/ItemIcon/medicinalherbs.medicinalherbs"),
		(L"/Game/ItemIcon/Water.Water"),
		(L"/Game/ItemIcon/Bottle.Bottle"),
		(L"/Game/ItemIcon/HPPotion.HPPotion"),
		(L"/Game/ItemIcon/Tomatojuice.Tomatojuice"),
		(L"/Game/ItemIcon/TomatoPotion.TomatoPotion"),
		(L"/Game/ItemIcon/BigHPPotion.BigHPPotion"),
		(L"/Game/ItemIcon/Nail.Nail"),
		(L"/Game/ItemIcon/Desk.Desk"),
		(L"/Game/ItemIcon/closet.closet"),
		(L"/Game/ItemIcon/TomatoBox.TomatoBox"),
		(L"/Game/ItemIcon/Sword.Sword"),
		(L"/Game/ItemIcon/Armor.Armor"),
		(L"/Game/ItemIcon/IronIngot.IronIngot"),
		(L"/Game/ItemIcon/Carbon.Carbon"),
		(L"/Game/ItemIcon/CarbonSteel.CarbonSteel"),
		(L"/Game/ItemIcon/God.God"),
		(L"/Game/ItemIcon/Sword3.Sword3"),
		(L"/Game/ItemIcon/Leather.Leather"),
		(L"/Game/ItemIcon/Thong.Thong"),
		(L"/Game/ItemIcon/Sword2.Sword2"),
		(L"/Game/ItemIcon/Sword5.Sword5"),
		(L"/Game/ItemIcon/Sword7.Sword7"),
		(L"/Game/ItemIcon/Sword4.Sword4"),
		(L"/Game/ItemIcon/Sword6.Sword6"),
		(L"/Game/ItemIcon/Armor4.Armor4"),
		(L"/Game/ItemIcon/Armor3.Armor3"),
		(L"/Game/ItemIcon/Armor2.Armor2"),
		(L"/Game/ItemIcon/all.all"),
		(L"/Game/ItemIcon/Attck.Attck"),
		(L"/Game/ItemIcon/def.def"),
		(L"/Game/ItemIcon/DEX.DEX"),
		(L"/Game/ItemIcon/STR.STR"),
		(L"/Game/ItemIcon/ekfrrktmatkf.ekfrrktmatkf"),
		(L"/Game/ItemIcon/rkach.rkach"),
		
	};

	for (int i = 0; i < TextureList::E_MAX; i++)
	{
		if (ItemIcon[i].Succeeded())
		{
			Textrues[i] = ItemIcon[i].Object;
		}
	}
}