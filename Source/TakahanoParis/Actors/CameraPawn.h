// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Actors/Interfaces/InteractInterface.h"
#include "GameFramework/Controller.h"
#include "CameraPawn.generated.h"

/**
 * @class ACameraPawn
 * @brief Describe an Actor that can be possessed and that is represented in the world as a Camera
 */
UCLASS()
class TAKAHANOPARIS_API ACameraPawn : public APawn , public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPawn();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	// InteractInterface
public:

	/**
	 *	@fn I_Use()
	 *	@brief Use function from Interact interface 
	 *	@return true if if succeded
	 *	@see IInteractInterface
	 */
	virtual bool I_Use(class AController * User) override;

	/**
	 *	@fn I_GetInteractState()
	 *	@brief Use function from Interact interface
	 *	@return the state of the interact-able 
	 *	@see IInteractInterface
	 */
	virtual EInteractableState I_GetInteractState() override;

	// end of InteractInterface

private:
	/**
	 *	@property CameraMesh
	 *	@brief StaticMesh representing the camera
	 *	@todo replace with a SkeletalMeh
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StaticMesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* CameraMesh;

	/**
	 *	@property ViewPoint
	 *	@brief CameraComponent used when switching to this actor
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StaticMesh, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* ViewPoint;


protected:

	/**
	 *	@property State
	 *	@brief State of this Actor
	 */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Player")
	EInteractableState State;


	/**
	 *	@property State
	 *	@brief State of this Actor
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		float CameraBlendTime;

	/**
	 * @fn SetAsViewTarget()
	 * @brief Adapt the view of the player controller to this camera
	 */
	UFUNCTION()
		void SetAsViewTarget(class APlayerController * PlayerController);
	
	
};
