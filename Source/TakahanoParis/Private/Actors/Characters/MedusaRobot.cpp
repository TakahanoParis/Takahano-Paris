// Fill out your copyright notice in the Description page of Project Settings.

#include "MedusaRobot.h"
#include "Kismet/KismetMathLibrary.h"


AMedusaRobot::AMedusaRobot() :Super() 
{
	AttackPoints = 100;
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

// @todo Set correct Damage types
void AMedusaRobot::Attack(AActor* Target)
{
	FDamageEvent Damage;
	Damage.IsOfType(0);

	if(GetController())
		Target->TakeDamage(AttackPoints, Damage, GetController(), this);

	Super::Attack(Target);
}
