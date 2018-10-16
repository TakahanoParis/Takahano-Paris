// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CustomHUD.generated.h"

/**
 * 
 */
UCLASS()
class TAKAHANOPARIS_API ACustomHUD : public AHUD
{
	GENERATED_BODY()


public:

	/** 
	 *	@fn DrawHUD()
	 *	@brief the draw loop to add functions to enable loop functions
	 *	from the AHUD class : "The Main Draw loop for the hud.  Gets called before any messaging.  Should be subclassed"
	 *	@see AHUD
	 */
	virtual void DrawHUD() override;


	/**
	 *	@fn FindActorsInCenterOfScreen()
	 *	@brief Get The actors in rectangle in the middle of screen
	 *	@param OutActors The array of Actors found at last redraw
	 *	@param ClassFilter the filter to limit Actors
	 *	@param CenterSpan The span of the rectangle from center of screen
	 *	@note You will only retrieve the actors from last draw
	 *	@todo Improve that logic, because it is highly inefficient to retrieve the last draw's found actors
	 */
	void GetActorsInCenterofScreen(TArray<AActor *> &OutActors, TSubclassOf<AActor> NewClassFilter, FVector2D NewCenterSpan);

private:

	/**
	 *	@property ActorsInSelection
	 *	@brief Contains the Actors in center of screen.
	 */
	UPROPERTY()
		TArray<AActor *> ActorsInCenterofScreen;

	/**
	 *	@property ClassFilter
	 *	@brief The class filter for the search of Actors in the middle of screen 
	 */
	UPROPERTY()
		TSubclassOf<AActor> ClassFilter;

	/**
	 *	@property ClassFilter
	 *	@brief The class filter for the search of Actors in the middle of screen
	 */
	UPROPERTY()
		FVector2D CenterSpan = FVector2D(100.f,100.f);

	/**
	 *	@property bArrayAsBeenUpdated
	 *	@brief boolean flag to know if DrawHUD() was called
	 */
	UPROPERTY()
		bool bArrayAsBeenUpdated;

	/**
	 *	@fn FindActorsInCenterOfScreen_Backend()
	 *	@brief Get The actors in rectangle in the middle of screen
	 *	@param OutActors The array of Actors found
	 *	@return true if something was found
	 *	@note we're using a backend to actually make the call only if necessary
	 */
	UFUNCTION()
		bool GetActorsInCenterofScreen_Backend(TArray<AActor *> &OutActors);








	
	
	
	
};
