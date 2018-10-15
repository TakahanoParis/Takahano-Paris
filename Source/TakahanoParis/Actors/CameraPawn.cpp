// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraPawn.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"


// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ACameraPawn::I_Use(AController * User)
{
	if(!User)
		return false;
	auto PC = Cast<APlayerController>(User);
	SetAsViewTarget(PC);
	return true;
}

EInteractableState ACameraPawn::I_GetInteractState()
{
	return State;
}

void ACameraPawn::SetAsViewTarget(APlayerController* PlayerController)
{
	FViewTargetTransitionParams VTParams;
	VTParams.BlendTime = CameraBlendTime;
	VTParams.BlendExp = 2.0;
	VTParams.BlendFunction = EViewTargetBlendFunction::VTBlend_EaseInOut;
	PlayerController->ClientSetViewTarget(this, VTParams);
}

void ACameraPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACameraPawn, State);
}