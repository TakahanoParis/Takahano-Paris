// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbableInterface.h"
#include "GameFramework/Actor.h"
#include "SideScrollerHero.h"

// Add default functionality here for any IClimbableInterface functions that are not pure virtual.
FVector IClimbableInterface::GetClimbTopTarget_Implementation() const
{
	return FVector(0.f);
}

void IClimbableInterface::OnCharacterIsClose_Implementation(ACharacter * PlayerPawn)
{
	const auto PlayerSCCharacter = Cast<ASideScrollerHero>(PlayerPawn);
	const auto AsActor = Cast<AActor>(this);
	PlayerSCCharacter->SetCanClimb(true, AsActor);
}

void IClimbableInterface::OnCharacterClimbed_Implementation(ACharacter* PlayerPawn)
{
	const auto PlayerSCCharacter = Cast<ASideScrollerHero>(PlayerPawn);
	const auto AsActor = Cast<AActor>(this);
	PlayerSCCharacter->SetCanClimb(false, nullptr);
}
