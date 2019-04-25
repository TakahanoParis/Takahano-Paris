// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbableInterface.h"
#include "GameFramework/Actor.h"
#include "SideScrollerHero.h"
#include "TP_SideScroller.h"

// Add default functionality here for any IClimbableInterface functions that are not pure virtual.
FVector IClimbableInterface::GetClimbTopTarget_Implementation() const
{
	return FVector(0.f);
}

void IClimbableInterface::OnCharacterIsClose_Implementation(ACharacter * PlayerPawn)
{
	const auto PlayerSCCharacter = Cast<ASideScrollerCharacter>(PlayerPawn);
	const auto AsActor = Cast<AActor>(this);
	if (!AsActor)
		UE_LOG(LogTP_SideScroller, Error, TEXT("Casting Climbable Interface into AActor failed"));
	PlayerSCCharacter->SetCanClimb(true, AsActor);
}


void IClimbableInterface::OnCharacterLeft_Implementation(ACharacter * PlayerPawn)
{
	const auto PlayerSCCharacter = Cast<ASideScrollerCharacter>(PlayerPawn);
	PlayerSCCharacter->SetCanClimb(false, nullptr);
}

void IClimbableInterface::OnCharacterClimbed_Implementation(ACharacter* PlayerPawn)
{
	const auto PlayerSCCharacter = Cast<ASideScrollerHero>(PlayerPawn);
	PlayerSCCharacter->SetCanClimb(false, nullptr);
}
