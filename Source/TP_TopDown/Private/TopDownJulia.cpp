// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownJulia.h"


bool ATopDownJulia::SetCharacter()
{
	bool IsValid = false;
	const auto julia = UTakahanoParisStatics::GetTakahanoParisJulia(IsValid);
	if (IsValid)
	{
		UTakahanoParisStatics::SetTakahanoParisCharacter(julia, GetMesh());
		return true;
	}
	return false;
}
