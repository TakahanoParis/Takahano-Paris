// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "Ability.h"
#include "UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


AHero::AHero() : Super()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Just set us a little off the center
	FollowCamera->SetRelativeLocation(FVector(0.f, -50.f, 0.f), false);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset (to avoid direct content references in C++)
}

void AHero::OnConstruction( const FTransform & Transform){
    Super::OnConstruction(Transform);
    if(Role == ROLE_Authority)
        for(auto it : AbilityClasses)
        {
            if(!it)
                return;
            UAbility* NewAbility = NewObject<UAbility>(this, it);
            if(!NewAbility)
                return;
            NewAbility->RegisterComponent();
            Abilities.Add(NewAbility);
        }

}

#if 0 // this code directly inherit from ANIMA and needs to be adapted to TakahanoParis naming scheme
void AHero::AerAbility()
{
	if (Abilities.IsValidIndex(0))
		Abilities[0]->TryUse();
}
#endif


void AHero::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AHero, Abilities);
}
