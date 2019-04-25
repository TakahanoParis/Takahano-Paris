// Fill out your copyright notice in the Description page of Project Settings.

#include "Footstep.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"



UFootstep::UFootstep()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Yellow;
#endif // WITH_EDITORONLY_DATA
	Intensity = 1.f;
	WalkingSurface = EPhysicalSurface::SurfaceType_Default;

	FootTraceDelegate.BindUObject(this, &UFootstep::OnTraceCompleted);

}

void UFootstep::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	SetWantTrace(MeshComp);
	if (bWantsTrace)
		RequestTrace(MeshComp);

	auto Surface = WalkingSurface;
	if (MeshComp && SoundCue)
	{
		UAudioComponent * FootStepAudio = UGameplayStatics::SpawnSoundAtLocation((UObject *)MeshComp->GetAttachmentRootActor(),
			SoundCue,
			MeshComp->GetComponentToWorld().GetLocation(),
			FRotator(),
			Intensity,
			1.f,
			1.f,
			SoundCue->AttenuationSettings,
			SoundCue->SoundConcurrencySettings,
			true);

		if (FootStepAudio)
		{
			FootStepAudio->SetIntParameter("Surface", (uint8)Surface);
			FootStepAudio->Play();
		}
		else
			UE_LOG(LogAudio, Warning, TEXT("Sound failed to be spawned"));
	}
	else
		UE_LOG(LogAudio, Error, TEXT("Sound cue not set in Animation"));
}

FTraceHandle UFootstep::RequestTrace(USkeletalMeshComponent *Creator)
{
	UWorld* World = Creator->GetWorld();
	if (World == nullptr)
		return FTraceHandle();

	ECollisionChannel Channel = ECollisionChannel::ECC_WorldStatic;
	FCollisionObjectQueryParams ObjectQueryParams(Channel);

	//bool bTraceComplex = false;
	//bool bIgnoreSelf = true;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Creator->GetAttachmentRootActor());

	static FName TraceName = FName(TEXT("MaterialTrace"));
	FCollisionQueryParams TraceParams(TraceName, true, nullptr);

	TraceParams.AddIgnoredActors(ActorsToIgnore);
	TraceParams.bTraceComplex = true;
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	auto Start = Creator->GetComponentToWorld().GetLocation();
	auto End = Start - FVector(0.f, 0.f, 500.f);

	return World->AsyncLineTraceByChannel(EAsyncTraceType::Single, Start, End, Channel, TraceParams, FCollisionResponseParams::DefaultResponseParam, &FootTraceDelegate);
}

void UFootstep::OnTraceCompleted(const FTraceHandle & Handle, FTraceDatum & Data)
{
	if(Handle == LastFootTrace)
	{
		UseTraceResults(Data);
		LastFootTrace._Data.FrameNumber = 0; // reset it
	}
	// Are we supposed to do this anyway ?
	LastFootTrace._Data.FrameNumber = 0; // reset it
}

void UFootstep::UseTraceResults(const FTraceDatum & TraceData)
{
	// do things here
	if (TraceData.OutHits.IsValidIndex(0))
	{
		if (TraceData.OutHits[0].PhysMaterial.IsValid())
		{
			WalkingSurface = UPhysicalMaterial::DetermineSurfaceType(TraceData.OutHits[0].PhysMaterial.Get());
		}
	}
}

void UFootstep::SetWantTrace(USkeletalMeshComponent * Creator)
{
	// don't allow overlapping traces here.
	if (!Creator->GetWorld()->IsTraceHandleValid(LastFootTrace, false))
		bWantsTrace = true;
}