// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeamInterface.generated.h"
/**
 *  @struct Team
 *  This Struct contains all relevant data about a Team
 */
USTRUCT(Blueprintable)
struct FTeam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int8 TeamNumber;

	UPROPERTY(EditAnywhere)
		FName TeamName;

	UPROPERTY(EditAnywhere)
		FColor TeamColor;

	FTeam()
	{
		TeamNumber = 0;
		TeamName = "";
	}
};


/**
 * Uinterface class
 * This class does not need to be modified.
 */
UINTERFACE(MinimalAPI, meta =(CannotImplementInterfaceInBlueprint))
class UTeamInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class ITeamInterface
 * Team interface. allows to ask objects about them belonging to a team.
 * This class is virtual and needs to be implemented to be in a working class
 * @see FTeam
 * @see ArenaGameMode
 */
class TAKAHANOPARIS_API ITeamInterface
{
	GENERATED_BODY()

public:

	/** 
	 * @brief I_GetTeam function.
 	 * @return the index of the team of your object.
	 * @note You MUST implement it in your classes
	 */
	UFUNCTION()
		virtual uint8 I_GetTeam() = 0;

	/**
	* @brief I_SetTeam function.
	* set the index of the team of your object.
	* @param NewTeam the index of the team you want to be in
	* @note You MUST implement it in your classes
	*/
	UFUNCTION()
		virtual void I_SetTeam(uint8 NewTeam) = 0;

	/**
	* @brief I_GetTeamBP function.
	* @return the index of the team of your object.
	* @note  for Blueprint, no need to implement it in your classes
	*/
	UFUNCTION(BlueprintCallable, meta=(DisplayName="GetTeam"))
		virtual int I_GetTeamBP();


	/**
	* @brief I_SetTeamBP function.
	* set the index of the team of your object;
	* @param NewTeam the index of the team you want to be in
	* @note  for Blueprint, no need to implement it in your classes
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetTeam"))
		virtual void I_SetTeamBP(uint8 NewTeam);

	/**
	* @brief  I_ValidateTeam function.
	* check if the FTeam you give is valid
	* @param Team The team you want checked
	* @param WorldContextObject a Valid UWorld will be fine
	* @return true if your team exist in the Game Mode
	* @note Cannot be a UFunction ( due to Static keyword)
	*/
	static bool I_ValidateTeam(FTeam Team, const UObject * WorldContextObject);
		
};
