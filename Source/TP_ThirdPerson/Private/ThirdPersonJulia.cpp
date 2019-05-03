// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonJulia.h"
//#include "GameFramework/Controller.h"
#include "Gameplay/CustomPlayerController.h"
#include "Actors/Interfaces/HackInterface.h"
#include "Actors/Interfaces/InteractInterface.h"
#include "UnrealNetwork.h"
#include "TakahanoParis.h"
#include "Kismet/GameplayStatics.h"
#include "TakahanoParisStatics.h"
#include "ActivableActor.h"
#include "HackCameraActor.h"

AThirdPersonJulia::AThirdPersonJulia() : Super()
{
	//HackDelegate.AddDynamic(this, &AThirdPersonJulia::Hack);
	PrimaryActorTick.bCanEverTick = true;
}




void AThirdPersonJulia::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AThirdPersonJulia::SetUsableActors()
{
	Super::SetUsableActors();
	if (GetUsableActors().IsValidIndex(GetTargetIndex()))
	{
		const AActivableActor * T = GetUsableActors()[GetTargetIndex()];
		if (T)
			UTakahanoParisStatics::SetPlayerTarget(this, T->GetUsableName(), T);
	}
	


}

void AThirdPersonJulia::MoveForward(float Value)
{
	if (!bUsingCamera)
		Super::MoveForward(Value);
}

void AThirdPersonJulia::MoveRight(float Value)
{
	if (!bUsingCamera)
		Super::MoveRight(Value);
}

void AThirdPersonJulia::Use()
{
	Super::Use();
	if (GetUsableActors().IsValidIndex(TargetIndex))
		if (GetUsableActors()[TargetIndex]->IsA<AHackCameraActor>())
		{
			bUsingCamera = true;
		}
}

void AThirdPersonJulia::Cancel()
{
	Super::Cancel();
	if (bUsingCamera)
	{
		bUsingCamera = false;
		Server_ReturnToCharacter();
	}

}

bool AThirdPersonJulia::SetCharacter()
{
	bool IsValid = false;
	const auto julia = UTakahanoParisStatics::GetTakahanoParisJulia(IsValid);
	if (IsValid)
	{
		UTakahanoParisStatics::SetTakahanoParisCharacter(julia, GetMesh());
		return true;
	}
	return false;
}


bool AThirdPersonJulia::Server_ReturnToCharacter_Validate(){return true;}
void AThirdPersonJulia::Server_ReturnToCharacter_Implementation()
{
	const auto aPC = Cast<APlayerController>(GetController());
	aPC->SetViewTargetWithBlend(this, 0.2);
	SetOwner(aPC);
	aPC->SetControlRotation(this->GetActorRotation());
	Controller = aPC;
	bIsUsingObject = false;
}

