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
class ACCTVActor : public APawn , public IHackInterface
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
		float BlendTime = 1.f;

	/**
	 *	@property BlendFunction
	 *	@brief Enum that gives the correct function to use to find the curve
	 */	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	//	 TEnumAsByte<EViewTargetBlendFunction> BlendFunction = EViewTargetBlendFunction::VTBlend_EaseInOut;

	/**
	 *	@property BlendTime
	 *	@brief float that is use to get the correct interpolation curve
	 */	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float BlendExponent = 1.f;

	////  group and name ;
protected :

	/**
	*	@property groupID
	*	@brief Camera Group. Used to limit the selection for the player. also to get an array of all the Camera in a group
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "Groupe Index"))
		int groupID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "CameraName"))
		FName CameraName;

public:

	/**
	 *	@fn GetCameraName
	 *	@returns Camera Name
	 */
	FORCEINLINE FName GetCameraName() const { return CameraName; }

	/**
	 *	@fn GetCameraName_BP
	 *	@returns Camera Name
	 *	@note For blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera", meta = (DisplayName = "Get Camera Name"))
		FName GetCameraName_BP() const { return GetCameraName(); }

	/**
	*	@fn GetCameraID
	*	@returns group ID
	*/
	FORCEINLINE int GetCameraID() const { return groupID; }

	/**
	*	@fn GetCameraID_BP
	*	@returns group ID
	*	@note For blueprints
	*/
	UFUNCTION(BlueprintCallable, Category = "Camera", meta = (DisplayName = "Get Camera group ID"))
		int GetCameraID_BP() const { return GetCameraID(); }


	/**
	*	@fn GetAllCameraWithID
	*	@brief Gets an Array of Camera belonging to the same ID group
	*	@param Cameras : the resulting Array
	*	@param ID :the ID to look for
	*/
	UFUNCTION()
		static void GetAllCameraWithID(const UObject * WorldContextObject, TArray<ACCTVActor *> &Cameras, int ID);


	/**
	*	@fn GetAllCameraWithID_BP
	*	@brief Gets an Array of Camera belonging to the same ID group
	*	@param Cameras : the resulting Array
	*	@param ID :the ID to look for
	*	@note For blueprintsS
	*/
	UFUNCTION(BlueprintCallable, Category = "Camera", meta = (DisplayName = "Get All Cameras with ID"))
		static void GetAllCameraWithID_BP(const UObject * WorldContextObject, TArray<ACCTVActor *> &Cameras, int ID) { GetAllCameraWithID(WorldContextObject, Cameras,ID); }

	/**
	*	@fn GetCameraWithName_BP
	*	@brief Get the Camera (or the first one), With a specified name
	*	@returns the wanted camera
	*	@param CameraName : the name you're looking for
	*/
	UFUNCTION()
		static ACCTVActor * GetCameraWithName(const UObject * WorldContextObject, FName SearchName);

	/**
	*	@fn GetCameraWithName_BP
	*	@brief Get the Camera (or the first one), With a specified name
	*	@returns the wanted camera
	*	@param CameraName : the name you're looking for
	*	@note For blueprints
	*/
	UFUNCTION(BlueprintCallable, Category = "Camera", meta = (DisplayName = "Get Camera with Name"))
		static ACCTVActor * GetCameraWithName_BP(const UObject * WorldContextObject, FName SearchName) {return GetCameraWithName(WorldContextObject, SearchName); }

	
	
};
