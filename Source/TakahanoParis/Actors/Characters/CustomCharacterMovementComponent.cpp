// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomCharacterMovementComponent.h"
#include "Curves/CurveFloat.h"
#include "UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

float UCustomCharacterMovementComponent::GetSpeedDelta() const
{
	if(bDeltaCalculatedInBlueprint)
		return GetSpeedDelta_BP();
	if (IsFalling())
		return 1.0;
	FFindFloorResult Floor;
	FindFloor(GetCharacterOwner()->GetCapsuleComponent()->GetComponentLocation(), Floor, true); /// may need to change true to false if it fails to work properly
	auto A = Floor.HitResult.ImpactNormal;
	auto B = GetCharacterOwner()->GetActorForwardVector();
	A.Normalize();
	B.Normalize();
	const auto degree = 90.0 - FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(A, B)));
	const float SlowRate = degree == 0 ? 1 : 1 / degree;
	const float TargetSpeedDelta = (bIsRunning? 1 : NormalSpeedDelta) * SlowRate;
	//UE_LOG(LogTemp, Warning, TEXT("TargetSpeed is : %f"), TargetSpeedDelta);
	return UKismetMathLibrary::FInterpTo(SpeedDelta, TargetSpeedDelta, UGameplayStatics::GetWorldDeltaSeconds(GetCharacterOwner()), 30.f);
}

float UCustomCharacterMovementComponent::GetMaxSpeed() const
{
	if (!WalkSpeedCurve)
		return Super::GetMaxSpeed();
	// Setup Speed Delta here
	return (MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking) ? WalkSpeedCurve->GetFloatValue(SpeedDelta) : Super::GetMaxSpeed();
}

void UCustomCharacterMovementComponent::PerformMovement(float DeltaTime)
{
	Super::PerformMovement(DeltaTime); // Called only on server, so beware ;)
	SpeedDelta = GetSpeedDelta();
}


void UCustomCharacterMovementComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCustomCharacterMovementComponent,SpeedDelta);
	DOREPLIFETIME(UCustomCharacterMovementComponent, bIsRunning);
	DOREPLIFETIME(UCustomCharacterMovementComponent, NormalSpeedDelta);
	DOREPLIFETIME(UCustomCharacterMovementComponent, RunInterpolationSpeed);
}
