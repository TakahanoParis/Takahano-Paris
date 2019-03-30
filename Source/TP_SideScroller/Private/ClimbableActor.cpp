// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbableActor.h"
#include "Components/BoxComponent.h"
#include "SideScrollerCharacter.h"
#include "Components/StaticMeshComponent.h"


AClimbableActor::AClimbableActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	ClimbTarget = CreateDefaultSubobject<USceneComponent>(TEXT("ClimbTarget"));
	ClimbTarget->SetupAttachment(RootComponent);

	PlayerDetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerDetectionBox"));
	PlayerDetectionBox->SetupAttachment(RootComponent);

	PlayerDetectionBox->OnComponentBeginOverlap.AddDynamic(this, &AClimbableActor::OnBoxOverlap);
}

void AClimbableActor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto SCChar = Cast<ASideScrollerCharacter>(OtherActor);
	if (!SCChar)
		return;
	IClimbableInterface::Execute_OnCharacterIsClose(this, SCChar);
}
