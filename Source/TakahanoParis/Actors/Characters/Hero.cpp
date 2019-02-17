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
#include "Kismet/GameplayStatics.h"

AHero::AHero() : Super()
{
	const auto GM = UGameplayStatics::GetGameMode(this);
	const auto CGM = Cast<ACustomGameMode>(GM);
	if(CGM)
		SetupCamera();

	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset (to avoid direct content references in C++)

	// Do not fall
	GetCharacterMovement()->bCanWalkOffLedges = false;

	// Move freely
	bUseControllerRotationYaw = false;
}


void AHero::BeginPlay()
{
	Super::BeginPlay();
	SetInteractableActors();
}

void AHero::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//SetVisibleInteractableActors();
}

void AHero::Use()
{
	SetVisibleInteractableActors();
	Use_BP();
}

void AHero::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AHero::Use);
}

void AHero::SetupCamera()
{

	if (!FollowCamera)
		FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (!CameraBoom)
		CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation

	FollowCamera->SetRelativeLocation(FVector(0.f, -50.f, 0.f), false);

	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

												   // Just set us a little off the center

	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.f;

	// Inherit rotation
	//CameraBoom->bInheritPitch = true;
	//CameraBoom->bInheritRoll = true;
	//CameraBoom->bInheritYaw = true;
	/*
	// Create a camera boom (pulls in towards the player if there is a collision)

	//CameraBoom->bInheritPitch = true;
	//CameraBoom->bInheritRoll = true;
	//CameraBoom->bInheritYaw = true;

	
	switch (ViewType)
	{
	case ECameraTypeEnum::CTE_ThirdPerson:



		break;
	case ECameraTypeEnum::CTE_TopDown:

		// Don't rotate character to camera direction
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;

		// Configure character movement
		GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
		GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
		GetCharacterMovement()->bConstrainToPlane = true;
		GetCharacterMovement()->bSnapToPlaneAtStart = true;

		// Create a camera boom...
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
		CameraBoom->TargetArmLength = 500.f;
		CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
		CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

											  // Create a camera...
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


		break;
	case ECameraTypeEnum::CTE_Side:
		// Create a follow camera
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
		break;
	default:
		break;
	}
	*/
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
	const auto aPS = Cast<ACustomPlayerState>(PlayerState);
	if (!aPS)
		return false;
	aPS->I_TakeDamage(DamageAmount, Instigator);
	return true;
}

AActor * AHero::GetClosestInteractableActor(float &Distance) const
{
	const auto Actors = GetAllInteractableActors();
	Distance = FVector::Distance(GetActorLocation(), Actors[0]->GetActorLocation());
	auto OutActor = Actors[0];
	for (auto it : Actors)
	{
		const auto T = FVector::Distance(GetActorLocation(), it->GetActorLocation());
		if (T < Distance)
		{
			Distance = T;
			OutActor = it;
		}
		
	}
	return OutActor;
}

void AHero::SetInteractableActors()
{
	UGameplayStatics::GetAllActorsWithInterface(this, UInteractInterface::StaticClass(), InteractableActors);
}

void AHero::SetVisibleInteractableActors()
{
	const auto aPC = Cast<ACustomPlayerController>(GetController());
	if(aPC)
	{
		VisibleInteractableActors.Empty();
		VisibleInteractableActors = InteractableActors;
		ACustomPlayerController::GetVisibleActorsInArray(VisibleInteractableActors, aPC);
	}

}

#if 0
// nothing is replicated in AHero
void AHero::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
#endif 
