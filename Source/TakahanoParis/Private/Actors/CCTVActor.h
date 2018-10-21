// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HackInterface.h"
#include "Viewport.h"
#include "CCTVActor.generated.h"

/**
 * @class ACCTVActor
 * @brief CPP class for all Close Circuit Camera. Julia Can hack into them to see the world
 * @see AJulia
 */
UCLASS()
class ACCTVActor : public AActor , public IHackInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCTVActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// IHackInterface functions
	virtual  bool I_Use(AController * User) override;
	virtual  void I_Hack(AController * User) override;
	virtual EInteractableState I_GetInteractState() const override {return InteractableState; }


protected:
	/**
	 *	@property InteractableState
	 *	@brief The state of its interaction abilities
	 */
	UPROPERTY(Replicated, EditAnywhere)
	EInteractableState InteractableState;

private:
	/**
	 *	@property CameraView
	 *	@brief Camera Component,  root of that actor
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent * CameraView;

	/**
	 *	@property CameraMesh
	 *	@brief The Mesh used to represent the Camera
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent * CameraMesh;

protected:

	/**
	 *	@brief float that is use to get the interpolation time.
	 */	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float BlendTime = 5.f;

	/**
	 *	@property BlendFunction
	 *	@brief Enum that gives the correct function to use to find the curve
	 */	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
		 TEnumAsByte<EViewTargetBlendFunction> BlendFunction = EViewTargetBlendFunction::VTBlend_EaseInOut;

	/**
	 *	@property BlendTime
	 *	@brief float that is use to get the correct interpolation curve
	 */	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float BlendExponent = 1.f;

	
	
};
