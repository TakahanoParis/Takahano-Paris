// Fill out your copyright notice in the Description page of Project Settings.
#include "CustomSaveGame.h"
#include "TakahanoParis.h"

UCustomSaveGame::UCustomSaveGame()
{
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
	PlayerLocation = FVector(0.f,0.f,0.f);
}

