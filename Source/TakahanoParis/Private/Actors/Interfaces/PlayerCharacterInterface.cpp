// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterInterface.h"
//#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/CustomPlayerController.h"
#include "InteractInterface.h"
#include "Kismet/GameplayStatics.h"


#if 0
void IPlayerCharacterInterface::I_SetReady(bool NewReady)
{
	const auto AsCharacter = Cast<ACharacter>(this);
	if (!AsCharacter)
		return;
	bool * ready = false;
	I_GetReadyByRef(ready);
	*ready = NewReady;
	UE_LOG(LogTemp, Warning, TEXT("SetReady Called (IsAuthority = %s)"), AsCharacter->HasAuthority() ? TEXT("True") : TEXT("False"));
	if (AsCharacter->Role == ROLE_AutonomousProxy)
		I_Server_IsReady(NewReady);
	I_OnRep_IsReady();
}

bool IPlayerCharacterInterface::I_GetIsReady() 
{
	bool * ready = false;
	I_GetReadyByRef(ready);
	return *ready;
}




bool IPlayerCharacterInterface::I_Server_IsReady_Validate(bool ServerIsReady)
{
	//TODO: Add some Verification that server allows ready state !!!! 
	return true;
}
void IPlayerCharacterInterface::I_Server_IsReady_Implementation(bool ServerIsReady)
{
	const auto AsCharacter = Cast<ACharacter>(this);
	if (!AsCharacter)
		return;
	bool * Server = false;
	I_GetReadyByRef(Server);
	*Server = ServerIsReady;
}

void IPlayerCharacterInterface::I_OnRep_IsReady()
{
	// Implements anything that would make sens here
}

void IPlayerCharacterInterface::I_SetCanJump(bool NewCanJump)
{
	I_Server_SetCanJump(NewCanJump);
}

bool  IPlayerCharacterInterface::I_Server_SetCanJump_Validate(bool NewCanJump) { return true; }
void  IPlayerCharacterInterface::I_Server_SetCanJump_Implementation(bool NewCanJump)
{
	const auto AsCharacter = Cast<ACharacter>(this);
	if (!AsCharacter)
		return;
	const auto Movement = Cast<UCharacterMovementComponent>(AsCharacter->GetMovementComponent());
		if (!Movement)
			return;
		Movement->SetJumpAllowed(NewCanJump);
}

AActor* IPlayerCharacterInterface::I_GetClosestInteractableActor(float& Distance) 
{
	const auto asPawn = Cast<ACharacter>(this);
	if (!asPawn)
		return nullptr;

	const auto location = asPawn->GetActorLocation();

	TArray<AActor *> AllInteractableActors = *I_GetAllInteractableArray();
	if(!AllInteractableActors.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("IPlayerCharacterInterface : I_GetClosestInteractableActor() : AllInteractableActors is empty"));
		return nullptr;
	}
		
	Distance = FVector::Distance(location, AllInteractableActors[0]->GetActorLocation());
	auto OutActor = AllInteractableActors[0];
	for (auto it : AllInteractableActors)
	{
		const auto T = FVector::Distance(location, it->GetActorLocation());
		if (T < Distance)
		{
			Distance = T;
			OutActor = it;
		}
	}
	return OutActor;
}

TArray<AActor*> IPlayerCharacterInterface::I_GetVisibleInteractableActors()
{
	if (!I_GetAllInteractableArray())
		return TArray<AActor *>();

	TArray<AActor *> VisibleInteractableActors;

	VisibleInteractableActors.Empty();
	const auto asPawn = Cast<ACharacter>(this);
	if(!asPawn)
		return TArray<AActor *>();

	const auto aPC = Cast<ACustomPlayerController>(asPawn->GetController());
	if (aPC)
	{
		VisibleInteractableActors = *I_GetAllInteractableArray();
		ACustomPlayerController::GetVisibleActorsInArray(VisibleInteractableActors, aPC);
	}

	return VisibleInteractableActors;
}

AActor * IPlayerCharacterInterface::I_GetClosestVisibleInteractableActor(float & Distance)
{
	const auto asPawn = Cast<ACharacter>(this);
	if (!asPawn)
		return nullptr;

	const auto location = asPawn->GetActorLocation();

	TArray<AActor *> VisibleInteractableActors = I_GetVisibleInteractableActors();

	Distance = FVector::Distance(location, VisibleInteractableActors[0]->GetActorLocation());
	auto OutActor = VisibleInteractableActors[0];
	for (auto it : VisibleInteractableActors)
	{
		const auto T = FVector::Distance(location, it->GetActorLocation());
		if (T < Distance)
		{
			Distance = T;
			OutActor = it;
		}
	}
	return OutActor;
}

void IPlayerCharacterInterface::I_SetAllInteractableActors(const UObject* WorldContextObject)
{
	TArray<AActor *> AllInteractableActors = *I_GetAllInteractableArray();
	UGameplayStatics::GetAllActorsWithInterface(WorldContextObject, UInteractInterface::StaticClass(), AllInteractableActors);
}

#endif