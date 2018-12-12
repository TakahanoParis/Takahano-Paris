// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomPlayerController.h"
#include "Actors/Interfaces/InteractInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/CustomPlayerState.h"


AHero::AHero() : Super()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//CameraBoom->bInheritPitch = true;
	//CameraBoom->bInheritRoll = true;
	//CameraBoom->bInheritYaw = true;

	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 0.5f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Just set us a little off the center
	FollowCamera->SetRelativeLocation(FVector(0.f, -50.f, 0.f), false);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset (to avoid direct content references in C++)

	// Do not fall
	GetCharacterMovement()->bCanWalkOffLedges = false;

	// Move freely
	bUseControllerRotationYaw = false;
}


void AHero::OnConstruction( const FTransform & Transform){
    Super::OnConstruction(Transform);
  }

bool AHero::TryUse(AActor * Target)
{
	auto AsInterface = Cast<IInteractInterface>(Target);
	if(!AsInterface)
		return false;
	switch(AsInterface->I_GetInteractState()) 
	{ 
	case EInteractableState::ISE_Off: 
		return false;
	case EInteractableState::ISE_Locked: 
		return false;
	case EInteractableState::ISE_Used: 	
		return false;
	case EInteractableState::ISE_Useable:
		if (GetController()->Role == ROLE_AutonomousProxy)
		{
			AsInterface->I_Server_Use(GetController());
			return true;
		}
		if (GetController()->Role == ROLE_Authority)
		{
			AsInterface->I_Server_Use_Implementation(GetController()); // We're already server, no need for confirmation
			return true;
		}
	default: 
		return false;
	}
}

bool AHero::I_TakeDamage(const float& DamageAmount, AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Take Tamage"));
	const auto aPS = Cast<ACustomPlayerState>(PlayerState);
	if (!aPS)
		return false;
	aPS->I_TakeDamage(DamageAmount, Instigator);
	return true;
}

bool AHero::CanRun()
{
	const auto aPS = Cast<ACustomPlayerState>(PlayerState);
	if (!aPS)
		return false;
	return aPS->GetStamina() >= 0;
}

void AHero::Run()
{
	const auto aPS = Cast<ACustomPlayerState>(PlayerState);
	if (!aPS)
		return;
	Super::Run();
}


#if 0
// nothing is replicated in AHero
void AHero::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
#endif 
