// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/EngineTypes.h"
#include "Actors/Interfaces/SaveableActorInterface.h"
#include "CustomPlayerController.generated.h"


// Forward Declaration
class UCustomWidget;

/**
 *	@brief ACustomPlayerController Class
 *	the base class for every player controller in Project centauri.
 */
UCLASS()
class TAKAHANOPARIS_API ACustomPlayerController : public APlayerController, public ISaveableActorInterface
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
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

	/**
* Called via input to turn at a given rate.
* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
*/
	virtual void Turn(float Val);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	virtual void LookUp(float Val);

public:


	/**
	 * @fn Get GetVisibleActorsInArray()
	 * @brief Get the actors implementing interface in Zone
	 * @param OutActors : an array containing valid scene actors
	 * @param Interface :the interface you want to use
	 * @return true if array is valid, false otherwise
	 */
	virtual bool GetVisibleActorsWithInterface(TArray<AActor*> &OutActors,const TSubclassOf<UInterface> Interface) const;

	/**
	 * @fn Get GetVisibleActorsInArray()
	 * @brief Get the actors implementing interface in Zone
	 * @param OutActors : an array containing valid scene actors
	 * @param Interface :the interface you want to use
	 * @return true if array is valid, false otherwise
	 * @note for blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "Actors", meta = (DisplayName = "Get Visible Actors With Interface"))
		virtual bool GetVisibleActorsWithInterface_BP(TArray<AActor*> &OutActors, const TSubclassOf<UInterface> Interface) const { return GetVisibleActorsWithInterface(OutActors, Interface); }


	/**
	 * @fn Get GetVisibleActorsInArray()
	 * @brief removes from array all actors that are not visible from player controller 
	 * @param OutActors : an array containing valid scene actors
	 * @param Player : The player that is is supposed to see the objects
	 * @return true if array is valid, false otherwise
	 */
	static bool GetVisibleActorsInArray(TArray<AActor*> &OutActors,const APlayerController * Player);


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
	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "AddWidgetToScreen_BP"))
		FORCEINLINE UCustomWidget * AddWidgetToScreen_BP(TSubclassOf<UCustomWidget> ClassToSpawn, FVector2D AnchorPoint = FVector2D(0.f,0.f), int ZOrder = 0) {return AddWidgetToScreen(ClassToSpawn, AnchorPoint, ZOrder); }

protected:

	/**
	 *	@property ActiveWidgetList
	 *	@brief Array containing all the Widget still on screen.
	 */
	UPROPERTY()
		TArray<UCustomWidget *> ActiveWidgetList;


private:

	/**
	 *	@property InitialCharacter
	 *	@brief the character originally controlled bty this pawn
	 */
	UPROPERTY(Replicated)
		class ABaseCharacter * InitialCharacter;

public:

	/**
	 *	@fn OnCharacterDie()
	 *	@brief Event to handle the death of a character
	 *	@note: this can be implemented and overriden in Blueprint
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay")
		void OnCharacterDie();

protected:

	/**
	 *	@fn Server_OnCharacterDie()
	 *	@brief Called on server 
	 */
	UFUNCTION(Server, Reliable,  WithValidation)
		virtual void Server_OnCharacterDie();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CutScene") 
		void PlayCutScene();


	virtual void ActorSaveDataSaved_Implementation(const FActorData &Data) override;

private:


	/**
	 *	@fn UpdatePlayersCharacterAlly
	 *	@brief Updates the list of friendly Pawns
	 */
	FORCEINLINE void UpdatePlayersCharacterAlly() { Server_UpdatePlayersCharacterAlly(); }

	/**
	 *	@fn Server_UpdatePlayersCharacterAlly
	 *	@brief Update on server the list of friendly Pawns
	 */
	UFUNCTION(Server, reliable, WithValidation)
		void Server_UpdatePlayersCharacterAlly();

	/**
	 *	@property FriedndlyPawns
	 *	@brief Array of Pawns that are friendly
	 */
	UPROPERTY(Replicated)
		TArray<class APawn * >  FriendlyPawns;

public:

	/**
	 *	@fn Direction2DToActor
	 *	@param Actor, the actor you're looking to indicate to player
	 *	@param out, a 2D vector indicating you where you should move
	 *	@brief Allow indicating where the player should move toward to find an actor
	 */
	UFUNCTION()
	bool Direction2DToActor(const AActor* Actor, FVector2D &Out) const;

	/**
	 *	@fn GetOtherPlayer
	 *	@brief gets pointer to other player pawn
	 *	@param Num as in the TArray.
	 */
	UFUNCTION()
		APawn* GetOtherPlayerPawn(int Num = 0) { return FriendlyPawns[Num]; }
};
