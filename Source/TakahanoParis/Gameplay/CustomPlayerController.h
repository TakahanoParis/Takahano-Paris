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

public:

	/**
	 *	@fn GetRelativeCursorCoordinate
	 *	@brief retrieve the coordinate of the middle of the screen
	 *	@return FVector2D : The pixel coordinates of the center of the screens
	 */
	UFUNCTION()
		FVector2D GetScreenCenterCoordinates();

	/**
	 *	@fn GetRelativeCursorCoordinate
	 *	@brief retrieve the coordinate of the middle of the screen
	 *	@return FVector2D : The pixel coordinates of the center of the screens
	 *	@note for Blueprints only
	 */
	UFUNCTION(BlueprintPure, Category = "Screen", meta = (DisplayName = "Get Screen Center coordinates"))
		FVector2D GetScreenCenterCoordinates_BP() { return GetScreenCenterCoordinates(); }


	/**
	 *	@fn GetRelativeCursorCoordinate
	 *	@brief	retrieve the Mouse position in a {(0,0) ,(1,1)} range via parameter
	 *	@param FVector2D Coordinate : The mouse position
	 *	@return bool : is the mouse inside the viewport
	 */
	UFUNCTION()
		bool GetAbsoluteCursorCoordinate(FVector2D &Coordinate);

	/**
	 *	@fn GetRelativeCursorCoordinate
	 *	@brief retrieve the coordinate of the cursor
	 *	retrieve the Mouse position in a {(0,0) ,(1,1)} range via parameter
	 *	@param FVector2D Coordinate : The mouse position
	 *	@return bool : is the mouse inside the viewport
	 *	@note for Blueprints only
	 */
	UFUNCTION(BlueprintPure, Category = "Mouse", meta = (DisplayName = "Get Relative Cursor Coordinates"))
		FORCEINLINE bool GetAbsoluteCursorCoordinate_BP(FVector2D &Coordinate) {return GetAbsoluteCursorCoordinate(Coordinate);}

	/**
	 *	@fn GetVisibleComponentUnderCursor()
	 *	@brief Fast and easy way of finding components under the mouse
	 *	@return USceneComponent * : A component that's under the cursor, nullptr if you're pointing at the void
	 */
	UFUNCTION()
		USceneComponent * GetVisibleComponentUnderCursor();

	/**
	*	@fn GetVisibleComponentUnderCursor_BP()
	*	@brief Fast an easy way of finding components under the mouse, for blueprints
	*	@return USceneComponent * : A component that's under the cursor, nullptr if you're pointing at the void
	*	@note : For Blueprints.
	*/
	UFUNCTION(BlueprintPure, Category = "Mouse", meta = (DisplayName = "Get Visible Component Under Cursor"))
		FORCEINLINE USceneComponent * GetVisibleComponentUnderCursor_BP() { return GetVisibleComponentUnderCursor(); }

	/**
	 *	@fn GetComponentByObjectUnderCursor()
	 *	@brief Fast an easy way of finding components under the mouse cursor
	 *	@return USceneComponent * : A component that's under the cursor, nullptr if you're pointing at the void
	 */
	UFUNCTION()
		USceneComponent * GetComponentByObjectUnderCursor();

	/**
	 *	@fn GetComponentByObjectUnderCursor_BP()
	 *	@brief Fast an easy way of finding components under the mouse cursor, for blueprints
	 *	@return USceneComponent * : A component that's under the cursor, nullptr if you're pointing at the void
	 *	@note : For Blueprints.
	 */
	UFUNCTION(BlueprintPure, Category = "Mouse", meta = (DisplayName = "Get Component by Object Under Cursor"))
		FORCEINLINE USceneComponent * GetComponentByObjectUnderCursor_BP() { return GetComponentByObjectUnderCursor(); }

	/**
	 *	@fn GetActorUnderCursor()
	 *	@brief Fast an easy way of finding Actor under the mouse cursor
	 *	@return AActor * : A component that's under the cursor, nullptr if you're pointing at the void
	 */
	UFUNCTION()
		AActor * GetActorUnderCursor();

	/**
	 *	@fn GetComponentByObjectUnderCursor_BP()
	 *	@brief Fast an easy way of finding Actor under the mouse cursor
	 *	@return AActor * : An Actor that's under the cursor, nullptr if you're pointing at the void
	 *	@note : For Blueprints.
	 */
	UFUNCTION(BlueprintPure, Category = "Mouse", meta = (DisplayName = "Get Actor Under Cursor"))
		FORCEINLINE AActor * GetActorUnderCursor_BP() { return GetActorUnderCursor(); }

	/**
	 *	@fn GetActorInCylinderScreenByChannel()
	 *	@brief Slow method but gets the Actor in a radius in screen.
	 *	@param FVector2D ScreenTraceLocation : the screen position at which you want to look for actors
	 *	@param float Radius : the radius from the screen position 
	 *	@return bool true if found something, False otherwise
	 */
	UFUNCTION()
		bool GetActorInCylinderScreenByChannel(TArray<AActor *> &ActorsInCylinder , FVector2D ScreenTraceLocation, float Radius, float Range, ECollisionChannel TraceChannel, bool bDrawDebug = false);

	/**
	 *	@fn GetActorInCylinderScreenByChannel_BP()
	 *	@brief Slow method but gets the Actor in a radius in screen.
	 *	@param FVector2D ScreenTraceLocation : the screen position at which you want to look for actors
	 *	@param float Radius : the radius from the screen position
	 *	@return bool true if found something, False otherwise
	 *	@note : For Blueprints.
	 */
	UFUNCTION(BlueprintPure, Category = "Screen", meta = (DisplayName = "Get Actor in cylinder from camera by channel "))
		FORCEINLINE bool GetActorInCylinderScreenByChannel_BP(TArray<AActor *> &ActorsInCylinder, FVector2D ScreenTraceLocation, float Radius, float Range, ECollisionChannel TraceChannel, bool bDrawDebug = false) { return GetActorInCylinderScreenByChannel(ActorsInCylinder, ScreenTraceLocation, Radius, Range, TraceChannel, bDrawDebug); }

	/**
	 *	@fn GetActorInConeScreen()
	 *	@brief Slow method but gets the Actor in a radius in screen.
	 *	@param FVector2D ScreenTraceLocation : the screen position at which you want to look for actors
	 *	@param float Radius : the radius from the screen position 
	 *	@return bool true if found something, False otherwise
	 */
	UFUNCTION()
		bool GetActorInCylinderScreenForObjects(TArray<AActor *> &ActorsInCylinder, FVector2D ScreenTraceLocation, float Radius, float Range, const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes, const TArray<AActor *> &ActorstoIgnore, bool bDrawDebug = false);

	/**
	 *	@fn GetActorInConeScreen_BP()
	 *	@brief Slow method but gets the Actor in a radius in screen.
	 *	@param FVector2D ScreenTraceLocation : the screen position at which you want to look for actors
	 *	@param float Radius : the radius from the screen position
	 *	@return bool true if found something, False otherwise
	 *	@note : For Blueprints.
	 */
	UFUNCTION(BlueprintPure, Category = "Screen", meta = (DisplayName = "Get Actor in cylinder from camera by objects "))
		FORCEINLINE bool GetActorInCylinderScreenForObjects_BP(TArray<AActor *> &ActorsInCylinder, FVector2D ScreenTraceLocation, float Radius, float Range, const TArray < TEnumAsByte < EObjectTypeQuery > > & ObjectTypes, const TArray<AActor *> &ActorstoIgnore, bool bDrawDebug = false) { return GetActorInCylinderScreenForObjects(ActorsInCylinder, ScreenTraceLocation, Radius, Range, ObjectTypes, ActorstoIgnore, bDrawDebug); }


	/**
	 *	@fn GetRenderedActors()
	 *	@brief Slow method but gets the Actor in a radius in screen.
	 *	@param CurrentlyRenderedActors : The List of Actors present in Screen
	 *	@param float MinRecentTime :Refresh time we should look for.
	 *	@return bool : true if something was found
	 */
	UFUNCTION()
		bool GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, const float MinRecentTime);

	/**
	 *	@fn GetRenderedActors()
	 *	@brief Slow method but gets the Actor in a radius in screen.
	 *	@param CurrentlyRenderedActors : The List of Actors present in Screen
	 *	@param float MinRecentTime :Refresh time we should look for.
	 *	@return bool : true if something was found
	 *	@note For Blueprints
	 */
	UFUNCTION(BlueprintPure, Category = "Screen", meta = (DisplayName = "Get Rendered Actors"))
		FORCEINLINE bool GetRenderedActors_BP(TArray<AActor*>& CurrentlyRenderedActors, const float MinRecentTime) { return GetRenderedActors(CurrentlyRenderedActors, MinRecentTime); }


	/**
	 *	@fn GetActorsInCenterOfScreen()
	 *	@brief Get Actors in the middle of the screen in a XMargin, Y Margin rectangle.
	 *	@param OutActors : The List of Actors found
	 *	@return bool : true if something was found
	 */
	template<typename ClassFilter>
		bool GetActorsInCenterOfScreen(TArray<ClassFilter *>& OutActors);

		/**
	 *	@fn GetActorsInCenterOfScreen()
	 *	@brief Get Actors in the middle of the screen in a XMargin, Y Margin rectangle.
	 *	@param OutActors : The List of Actors found
	 *	@param ClassFilter : Class you want to limit the search to
	 *	@return bool : true if something was found
	 *	@note : this function will be slower than the template version
	 */
	UFUNCTION(BlueprintPure, Category = "Screen", meta = (DisplayName = "Get Actor in center of Screen"))
		bool GetActorsInCenterOfScreen_BP(TArray<AActor *>& OutActors, TSubclassOf<AActor> ClassFilter)
	{
		OutActors.Empty();
		const bool R = GetActorsInCenterOfScreen<AActor>(OutActors);
		for(auto EachActor : OutActors)
			if (!EachActor->IsA(ClassFilter))
				OutActors.Remove(EachActor);
		return R;
	}


protected :

	/**
	 *	@property CenterOfScreenSpan
	 *	@brief Span for the GetActorsInCenterOfScreen() function
	 *	@see GetActorsInCenterOfScreen()
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Screen", meta = (DisplayName = "Span for Get Actor in center of Screen"))
		FVector2D CenterOfScreenSpan;


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
		
};
