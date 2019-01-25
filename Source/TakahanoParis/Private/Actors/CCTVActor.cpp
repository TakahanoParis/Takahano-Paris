// Fill out your copyright notice in the Description page of Project Settings.

#include "CCTVActor.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealNetwork.h"


// Sets default values
ACCTVActor::ACCTVActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	PrimaryActorTick.bCanEverTick = true;
	CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera View"));
	CameraView->SetupAttachment(RootComponent);
	CameraView->bAutoActivate = true;
	CameraView->bUsePawnControlRotation = true;
	//RootComponent = CameraView;
	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Camera Mesh"));
	CameraMesh->SetupAttachment(CameraView);


}

// Called when the game starts or when spawned
void ACCTVActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACCTVActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACCTVActor::I_Use(AController* User)
{
	I_Use_BP(User);
	return false ;
}

void ACCTVActor::I_Hack(AController* User)
{
	I_Use_BP(User);
	//testIfPlayerController
	const auto aPC = Cast<APlayerController>(User);
	if(!aPC)
		return;
	aPC->SetViewTargetWithBlend(this, BlendTime, VTBlend_EaseInOut, BlendExponent, false);
	SetOwner(aPC);
	aPC->SetControlRotation(this->GetActorRotation());
	Controller = aPC;
	//aPC->Possess(this);
}


void ACCTVActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCTVActor, InteractableState);
}
