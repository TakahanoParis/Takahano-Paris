// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/Character.h"

#include "GameModeInterface.generated.h"




// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TAKAHANOPARIS_API IGameModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Multiplayer")
		AController *  AddSplitScreenPlayer();
	virtual AController *  AddSplitScreenPlayer_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Multiplayer")
		ACharacter * ReplaceCharacter(AController * InController, TSubclassOf<ACharacter>NewPawnClass);
	virtual ACharacter * ReplaceCharacter_Implementation(AController * InController, TSubclassOf<ACharacter>NewPawnClass);

	/**
	*	@fn GetDefaultPlayerTeamID
	*	@brief Getter for the DefaultPlayerTeamID
	*	@return The Team ID for the new IA players
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Teams", meta = (DisplayName = "Get Default AI Team ID"))
		uint8 GetDefaultPlayerTeamID() const;
	virtual uint8 GetDefaultPlayerTeamID_Implementation() const;

	/**
	*	@fn GetDefaultAITeamID
	*	@brief Getter for the DefaultAITeamID
	*	@return The Team ID for the new IA players
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Teams", meta = (DisplayName = "Get Default AI Team ID"))
		uint8 GetDefaultAITeamID() const;
	virtual uint8 GetDefaultAITeamID_Implementation() const;


	/**
	*	@fn OnGameOver_BP
	*	@brief End Level related event
	*	@note For Blueprints
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable,  meta = (DisplayName = "On Game Over"))
		void OnGameOver_BP();





};
