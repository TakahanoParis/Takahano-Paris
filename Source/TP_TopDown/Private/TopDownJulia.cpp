// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownJulia.h"
#include "TP_TopDown.h"


bool ATopDownJulia::SetCharacter()
{
	bool IsValid = false;
	const auto julia = UTakahanoParisStatics::GetTakahanoParisJulia(IsValid);
	if (IsValid)
	{
		UTakahanoParisStatics::SetTakahanoParisCharacter(julia, GetMesh());
		return true;
	}
	UE_LOG(LogTP_TopDown, Error, TEXT("Could not set Julia, Couldn't retrieve setup from singleton"));
	return false;
}
