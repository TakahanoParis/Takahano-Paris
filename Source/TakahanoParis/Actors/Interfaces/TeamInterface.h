// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeamInterface.generated.h"

class AActor;

/**
 *	@enum ETeamAttitudeEnum
 *	@brief describe how the team interacts with 
 */
UENUM(BlueprintType)
enum class ETeamAttitudeEnum  : uint8
{
    TAE_Hostile 	UMETA(DisplayName="Hostile"),
    TAE_Friendly 	UMETA(DisplayName="Friendly"),
    TAE_Neutral 	UMETA(DisplayName="Neutral")
}; 


/**
 *  @struct FTeam
 *  This Struct contains all relevant data about a Team
 */
USTRUCT(BlueprintType)
struct TAKAHANOPARIS_API FTeam
{
	GENERATED_USTRUCT_BODY()

private:
	enum EPredefinedId
	{
		PlayersCoopCharacter = 0,
		NoTeamId = 255	// if you want to change NoTeam's ID update FGenericTeamId::NoTeam
	};

protected:
	UPROPERTY(Category = "TeamID", EditAnywhere, BlueprintReadWrite)
	uint8 TeamID;

public:
	FTeam(uint8 InTeamID = NoTeamId) : TeamID(InTeamID)
	{}

	FORCEINLINE operator uint8() const { return TeamID; }

	FORCEINLINE uint8 GetId() const { return TeamID; }

	FORCEINLINE void SetId(uint8 NewId) { TeamID = NewId; }
	
	static FTeam GetTeamIdentifier(const AActor* TeamMember);
	static const ETeamAttitudeEnum GetAttitude(const AActor* A, const AActor* B);
	static const ETeamAttitudeEnum GetAttitude(const FTeam TeamA,const FTeam TeamB)
	{
		return AttitudeSolverImpl ? (*AttitudeSolverImpl)(TeamA, TeamB) : ETeamAttitudeEnum::TAE_Neutral;
	}

	typedef ETeamAttitudeEnum FTeamAttitudeSolverFunction(FTeam, FTeam);
	
	static void SetAttitudeSolver(FTeamAttitudeSolverFunction* Solver)	{AttitudeSolverImpl = Solver;}

	static ETeamAttitudeEnum DefaultTeamAttitudeSolver(FTeam A, FTeam B);


protected:
	// the default implementation makes all teams hostile
	// @note that for consistency IGenericTeamAgentInterface should be using the same function 
	//	(by default it does)
	static FTeamAttitudeSolverFunction* AttitudeSolverImpl;

public:
	static const FTeam NoTeam;
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
		virtual FTeam I_GetTeam() const = 0;

protected:
	/**
	* @brief I_SetTeam function.
	* set the index of the team of your object.
	* @param NewTeam the index of the team you want to be in
	* @note You MUST implement it in your classes
	*/
	UFUNCTION()
		virtual void I_SetTeam(FTeam NewTeam) = 0;

public:

	UFUNCTION(Server, Reliable, WithValidation)
		virtual void I_Server_SetTeam(FTeam NewTeam) ;
		virtual void I_Server_SetTeam_Implementation(FTeam NewTeam);

	/**
	* @brief I_GetTeam_BP function.
	* @return the index of the team of your object.
	* @note  for Blueprint, no need to implement it in your classes
	*/
	UFUNCTION(BlueprintCallable, meta=(DisplayName="GetTeam"))
		virtual int I_GetTeam_BP() { return I_GetTeam().GetId(); }


	/**
	* @brief I_SetTeam_BP()
	* set the index of the team of your object;
	* @param NewTeam the index of the team you want to be in
	* @note  for Blueprint, no need to implement it in your classes
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetTeam"))
		virtual void I_SetTeam_BP(int NewTeam){I_Server_SetTeam(static_cast<uint8>(NewTeam)); }


	/**
	 *	@fn I_GetAttitudeToward()
	 *	@brief Gets the attitude toward other actor
	 *	@return attitude towards OtherTeamActor
	 *	@param OtherTeamActor the other actor we wanna know the attitude towards
	 */
	UFUNCTION()
		virtual ETeamAttitudeEnum I_GetAttitudeToward(AActor * OtherTeamActor) const;

	/**
	 *	@fn I_GetAttitudeToward_BP()
	 *	@brief Gets the attitude toward other actor
	 *	@return attitude towards OtherTeamActor
	 *	@param OtherTeamActor the other actor we wanna know the attitude towards
 	 *	@note  for Blueprint, no need to implement it in your classes
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Attitude toward"))
		virtual ETeamAttitudeEnum I_GetAttitudeToward_BP(AActor * OtherTeamActor) const { return I_GetAttitudeToward(OtherTeamActor); }

};
