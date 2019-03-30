// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownCymie.h"


bool ATopDownCymie::SetCharacter()
{
	bool IsValid = false;
	const auto cymie = UTakahanoParisStatics::GetTakahanoParisCymie(IsValid);
	if (IsValid)
	{
		UTakahanoParisStatics::SetTakahanoParisCharacter(cymie, GetMesh());
		return true;
	}
	return false;
}
