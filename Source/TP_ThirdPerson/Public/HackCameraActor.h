// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActivableActor.h"
#include "HackCameraActor.generated.h"



/**
 *	ACameraActor class  
 *	An actor you can use to see the world through
 */
UCLASS(ClassGroup = (Actor))
class TP_THIRDPERSON_API AHackCameraActor : public AActivableActor
{
	GENERATED_BODY()

public:

	/**
	 * @brief Default Constructor
	 */
	AHackCameraActor();


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
		float BlendTime = 1.f;

	/**
	 *	@property BlendTime
	 *	@brief float that is use to get the correct interpolation curve
	 */	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float BlendExponent = 1.f;

protected :

	/**
	*	@property groupID
	*	@brief Camera Group. Used to limit the selection for the player. also to get an array of all the Camera in a group
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "Floor Index"))
		uint8 FloorID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "CameraName"))
		FName CameraName;




public :
	virtual void Use_Implementation(AThirdPersonCharacter * PlayerCharacter, bool &bShouldLockMovement) override;

	virtual bool CanUse(AThirdPersonCharacter* UserCharacter) override;

	FORCEINLINE void SetFloorIndex(uint8 NewFloorID) {FloorID = NewFloorID;	}

	UFUNCTION(BlueprintPure)
		FORCEINLINE uint8 GetFloorIndex() const { return FloorID; }


private :
	void SetViewTarget(APlayerController * PlayerController);

public:

	virtual FName GetUsableName() const override;


};

