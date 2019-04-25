// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownCymie.h"
#include "TP_TopDown.h"

bool ATopDownCymie::SetCharacter()
{
	bool IsValid = false;
	const auto cymie = UTakahanoParisStatics::GetTakahanoParisCymie(IsValid);
	if (IsValid)
	{

		IsValid = UTakahanoParisStatics::SetTakahanoParisCharacter(cymie, GetMesh());
#if WITH_EDITOR
		UE_LOG(LogTP_TopDown, Display, TEXT("Cymie setup from singleton : %d"), (int)IsValid);
#endif // WITH_EDITOR
		return IsValid;	
	}
	UE_LOG(LogTP_TopDown, Error, TEXT("Could not set Cymie, Couldn't retrieve setup from singleton"));
	return false;
}
