// Fill out your copyright notice in the Description page of Project Settings.

#include "MedusaRobot.h"


AMedusaRobot::AMedusaRobot() :Super() 
{
}

void AMedusaRobot::SetAIEnable(bool bEnable)
{
	const auto aAIC = GetCustomAIController();
	if(aAIC)
	{
		if (bEnable)
		{
			aAIC->StartAILogic();
			return;
		}
		aAIC->StopAILogic();
	}
}
