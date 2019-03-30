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

AThirdPersonJulia::AThirdPersonJulia() : Super()
{
	//HackDelegate.AddDynamic(this, &AThirdPersonJulia::Hack);
	PrimaryActorTick.bCanEverTick = true;
	bIsUsingObject = false;
}




void AThirdPersonJulia::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetVisibleHackables();
}

void AThirdPersonJulia::MoveForward(float Value)
{
	if(bIsUsingObject)
	{
		if (UsedActor)
			return;
	}
	Super::MoveForward(Value);
}

void AThirdPersonJulia::MoveRight(float Value)
{
	if (bIsUsingObject)
	{
		if(UsedActor)	
			return;
	}
	Super::MoveRight(Value);
}

void AThirdPersonJulia::BeginPlay()
{
	Super::BeginPlay();
	Hackables.Empty();
	UGameplayStatics::GetAllActorsWithInterface(this, UHackInterface::StaticClass(), Hackables);
}

void AThirdPersonJulia::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("NextTarget", IE_Pressed, this, &AThirdPersonJulia::IncrementTarget);
	PlayerInputComponent->BindAction("PreviousTarget", IE_Pressed, this, &AThirdPersonJulia::DecrementTarget);
}


bool AThirdPersonJulia::SetVisibleHackables()
{
	const auto aPC = Cast<APlayerController>(GetController());
	if(aPC)
	{
		VisibleHackables = Hackables;
		ACustomPlayerController::GetVisibleActorsInArray(VisibleHackables, aPC);
	}
	return VisibleHackables.Num() > 0;
}

void AThirdPersonJulia::IncrementTarget()
{
	if (VisibleHackables.IsValidIndex(TargetIndex + 1))
		TargetIndex++;
	else
		TargetIndex = 0;
}

void AThirdPersonJulia::DecrementTarget()
{
	if (VisibleHackables.IsValidIndex(TargetIndex - 1))
		TargetIndex--;
	else
		TargetIndex = VisibleHackables.Num()-1;
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

AActor* AThirdPersonJulia::GetHackTarget() const
{
	if (VisibleHackables.IsValidIndex(TargetIndex))
		return VisibleHackables[TargetIndex];
	return nullptr;
}


bool AThirdPersonJulia::TryHack(AActor* target)  
{
	auto AsInterface = Cast<IHackInterface>(target);
	HackDelegate.Broadcast(target);
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
			Server_Hack(target);
			return true;
		}
		if (GetController()->Role == ROLE_Authority)
		{
			Server_Hack_Implementation(target);
			return true;
		}
	default: 
		return false;
	}
}

bool AThirdPersonJulia::Server_ReturnToCharacter_Validate()
{
	return true;
}

void AThirdPersonJulia::Server_ReturnToCharacter_Implementation()
{
	const auto aPC = Cast<APlayerController>(GetController());
	aPC->SetViewTargetWithBlend(this, 0.2);
	SetOwner(aPC);
	aPC->SetControlRotation(this->GetActorRotation());
	Controller = aPC;
	bIsUsingObject = false;
	UsedActor = nullptr;
}

bool AThirdPersonJulia::Server_Hack_Validate(AActor * target)
{
	return true;
}


void AThirdPersonJulia::Server_Hack_Implementation (AActor * target)
{
	if(UTakahanoParisStatics::CallHackInterfaceOnActor(target, GetController()))
	{
		bIsUsingObject = true;
		UsedActor = target;
	}
}

void AThirdPersonJulia::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AThirdPersonJulia, bIsUsingObject);
	DOREPLIFETIME(AThirdPersonJulia, UsedActor);
	DOREPLIFETIME(AThirdPersonJulia, Hackables);
	DOREPLIFETIME(AThirdPersonJulia, Target);
}
