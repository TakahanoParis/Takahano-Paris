// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterLifeInterface.h"

float ICharacterLifeInterface::I_GetLifePoints() const
{
	const auto AsCharacter = Cast<ACharacter>(this);
	if (!AsCharacter)
		return -1.f;
	const auto aPS = Cast<ACustomPlayerState>(AsCharacter->GetController()->PlayerState);
	if (aPS)
		return aPS->I_GetLifePoints();
	return -1.f;
}

bool ICharacterLifeInterface::I_TakeDamage(const float & DamageAmount, AActor * Instigator)
{
	// as a default we only consider the Interface for classes deriving from character 
	const auto AsCharacter = Cast<ACharacter>(this);
	if (!AsCharacter)
		return false;
	UE_LOG(LogTemp, Warning, TEXT("Character %s Taking %f damages"), *AsCharacter->GetName(), DamageAmount);
	const auto aPS = Cast<ACustomPlayerState>(AsCharacter->GetController()->PlayerState);
	if (aPS)
		return aPS->I_TakeDamage(DamageAmount, Instigator);
	return false;
}
