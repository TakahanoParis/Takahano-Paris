// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownHero.h"
#include "Components/InputComponent.h"

void ATopDownHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(UTakahanoParisStatics::Dialogue, IE_Released, this, &ATopDownHero::Use);
}
