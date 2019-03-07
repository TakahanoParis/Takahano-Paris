// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterInterface.h"
//#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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