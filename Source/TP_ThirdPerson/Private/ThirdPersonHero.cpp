// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonHero.h"
#include "TP_ThirdPerson.h"
#include "UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomPlayerController.h"
#include "Actors/Interfaces/InteractInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/CustomPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "ActivableActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"


AThirdPersonHero::AThirdPersonHero() : Super(), TargetIndex(0)
{
	// Do not fall
	GetCharacterMovement()->bCanWalkOffLedges = false;
	// Move freely
	bUseControllerRotationYaw = false;
}


void AThirdPersonHero::BeginPlay()
{
	Super::BeginPlay();
	SetInteractableActors();
}

void AThirdPersonHero::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetUsableActors();
	if (GetUsableActors().IsValidIndex(TargetIndex))
	{
		const auto target = GetUsableActors()[TargetIndex];
		UTakahanoParisStatics::SetPlayerTarget(this, target->GetUsableName(), target);
		UTakahanoParisStatics::SetPlayerHelp(this, UTakahanoParisStatics::GetUseHelpText(), target);
	}
	else
	{
		UTakahanoParisStatics::SetPlayerTarget(this, FName(), nullptr);

		UTakahanoParisStatics::SetPlayerHelp(this, FText(), nullptr);
	}
}

void AThirdPersonHero::Use()
{
	Super::Use();
	if (GetUsableActors().IsValidIndex(TargetIndex))
	{
		GetUsableActors()[TargetIndex]->Use(this, bIsUsingObject);
#if WITH_EDITOR
		UE_LOG(LogTP_ThirdPerson, Warning, TEXT("%s is using %s"), *GetName(), *GetUsableActors()[TargetIndex]->GetName());
#endif
	}
}


bool AThirdPersonHero::I_TakeDamage(const float& DamageAmount, AActor* Instigator)
{
	const auto aPS = Cast<ACustomPlayerState>(PlayerState);
	if (!aPS)
		return false;
	aPS->I_TakeDamage(DamageAmount, Instigator);
	return true;
}

void AThirdPersonHero::SetInteractableActors()
{
	UGameplayStatics::GetAllActorsOfClass(this, AActivableActor::StaticClass(), reinterpret_cast<TArray<AActor*>&>(InteractableActors));
}

void AThirdPersonHero::SetVisibleInteractableActors()
{
	if (!GetController()) return;
	VisibleInteractableActors = InteractableActors;
	FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
	CollisionParams.AddIgnoredActor(this);
	for (int32 id = VisibleInteractableActors.Num() - 1; id >= 0; --id)
	{
		const auto it = VisibleInteractableActors[id];

		FVector ViewLocation;	FRotator ViewRotator;
		GetController()->GetPlayerViewPoint(ViewLocation, ViewRotator);
		const FVector ViewVector = UKismetMathLibrary::GetForwardVector(ViewRotator);
		FVector ToOtherActor = it->GetActorLocation() - ViewLocation;
		ToOtherActor.Normalize();

		// Forget about actors behind us :
		if (FVector::DotProduct(ToOtherActor, ViewVector) < 0)
		{
			VisibleInteractableActors.RemoveAt(id, 1, false);
			continue;
		}

		struct FHitResult OutHit;
		CollisionParams.AddIgnoredActor(it);

		GetWorld()->LineTraceSingleByChannel(OutHit, it->GetActorLocation(), ViewLocation,ECC_Visibility , CollisionParams);
		if (OutHit.IsValidBlockingHit())
			VisibleInteractableActors.RemoveAt(id, 1, false);
	}
	if (VisibleInteractableActors.Num()>0)	VisibleInteractableActors.Shrink();
}

void AThirdPersonHero::SetUsableActors()
{
	//SetInteractableActors(); // in case anything changed
	SetVisibleInteractableActors(); // let's figure out which we see
	UsableActors.Empty();
	for (auto it : VisibleInteractableActors)
	{
		if (it)
			if (it->CanUse(this))
			{
				UsableActors.Add(it);
			}
	}
}

void AThirdPersonHero::MoveForward(float Value)
{
	if (bIsUsingObject)
			return;
	Super::MoveForward(Value);
}

void AThirdPersonHero::MoveRight(float Value)
{
	if (bIsUsingObject)
			return;
	Super::MoveRight(Value);
}


// nothing is replicated in AThirdPersonHero
void AThirdPersonHero::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AThirdPersonHero, bIsUsingObject);
	DOREPLIFETIME(AThirdPersonHero, InteractableActors);
}

