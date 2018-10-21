// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplinePathActor.generated.h"

class USplineComponent;

/**
 * @class ASplinePathActor
 * @brief The Class of actor to describe a path to follow for AI
 * @note this actor works only if you have a valid navMesh.
 */
UCLASS()
class TAKAHANOPARIS_API ASplinePathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplinePathActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	
	/**
	 * @fn GetSpline()
	 * @brief Getter for the Spline component
	 * @param DistanceDelta : How far along the spline you are
	 */
	UFUNCTION()
	FORCEINLINE FVector GetWorldLocationAlongSpline(float DistanceDelta) const;


	/**
	 * @fn GetSpline()
	 * @brief Getter for the Spline component
	 */
	UFUNCTION()
	FORCEINLINE USplineComponent * GetSpline() const {return Spline;}

	/**
	 * @fn GetPathPoints()
	 * @brief Getter for the Spline component
	 */
	UFUNCTION()
	FORCEINLINE int GetPathPoints() const {return PathSteps;}


protected:
	/**
	 * @property Spline
	 * @brief The actual path
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	USplineComponent * Spline;

	/**
	 * @property PathSteps
	 * @brief number of steps in the path
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	int PathSteps;
		
	
	
};
