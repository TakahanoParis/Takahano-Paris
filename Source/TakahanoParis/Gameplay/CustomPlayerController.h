// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/EngineTypes.h"
#include "CustomPlayerController.generated.h"

// Forward Declaration
class UCustomWidget;

/**
 *	@brief ACustomPlayerController Class
 *	the base class for every player controller in Project centauri.
 */
UCLASS()
class TAKAHANOPARIS_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Default Constructor */
	ACustomPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	/**
	 *	@fn SetupInputComponent function override
	 *	@brief Enable the bindings for the controls
	 *	@see APlayerController
	 */
	virtual void SetupInputComponent() override;

	virtual void Reset() override;

protected:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/**
   * Called via input to turn at a given rate.
   * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
   */
	virtual void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	virtual void LookUpAtRate(float Rate);

public:

	virtual bool GetVisibleActorsWithInterface(TArray<AActor*> &OutActors,const TSubclassOf<UInterface> Interface) const;

	/**
	 * @fn Get GetVisibleActorsInArray()
	 * @brief removes from array all actors that are not visible from player controller 
	 * @param OutActors : an array containing valid scene actors
	 * @param Player : The player that is is supposed to see the objects
	 * @return true if array is valid, false otherwise
	 */
	static bool GetVisibleActorsInArray(TArray<AActor*> &OutActors,const ACustomPlayerController * Player);



	// Widgets functions
public:

	/**
	 *	@fn AddWidgetToScreen()
	 *	@param ClassToSpawn : The class of the Custom Widget
	 *	@param AnchorPoint	: The @D position on screen at which we should add it to viewport
	 *	@return UCustomWidget * , a pointer to the newly created Widget 
	 */
	UFUNCTION()
		UCustomWidget * AddWidgetToScreen(TSubclassOf<UCustomWidget> ClassToSpawn, FVector2D AnchorPoint = FVector2D(), int ZOrder = 0);

	/**
	 *	@fn AddWidgetToScreen_BP()
	 *	@param ClassToSpawn : The class of the Custom Widget
	 *	@param AnchorPoint	: The @D position on screen at which we should add it to viewport
	 *	@return UCustomWidget * , a pointer to the newly created Widget
	 */
	UFUNCTION()
		UCustomWidget * AddWidgetToScreen_BP(TSubclassOf<UCustomWidget> ClassToSpawn, FVector2D AnchorPoint = FVector2D(), int ZOrder = 0) {return AddWidgetToScreen(ClassToSpawn, AnchorPoint, ZOrder); }

protected:

	/**
	 *	@property ActiveWidgetList
	 *	@brief Array containing all the Widget still on screen.
	 */
	UPROPERTY()
		TArray<UCustomWidget *> ActiveWidgetList;


private:
	UPROPERTY(Replicated)
		class ABaseCharacter * InitialCharacter;


};
