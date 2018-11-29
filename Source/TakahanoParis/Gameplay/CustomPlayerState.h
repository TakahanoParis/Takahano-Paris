// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Actors/Interfaces/TeamInterface.h"
#include "Actors/Interfaces/LifeInterface.h"
#include "CustomPlayerState.generated.h"


/**
 *	@class ACustomPlayerState
 *	@brief player infos : Life points, stamina, etc...
 *	@note all the handling of the info could be handled by the character but the default design of UE4 is to have this separate class
 */
UCLASS()
class TAKAHANOPARIS_API ACustomPlayerState : public APlayerState, public ITeamInterface, public ILifeInterface
{
	GENERATED_BODY()


public :

	/**	@fn Default Constructor  */
	ACustomPlayerState();

	/**	@see APlayerState */
	void ClientInitialize(AController* C) override;

private :
	AController * Owner;

protected:

	/**
	 *	@property PlayerTeam
	 *	@brief Team this player belongs to 
	 */
	UPROPERTY(Replicated)
		FTeam PlayerTeam;


	/**
	 *	@property PlayerLife
	 *	@brief How much Life this player has
	 */
	UPROPERTY(Replicated)
		float PlayerLife;


	/**
	 *	@property PlayerStamina
	 *	@brief How much stamina this player has
	 */
	UPROPERTY(Replicated)
		float PlayerStamina;

	/**
	 *	@property MaxPlayerLife
	 *	@brief how much life this player can have
	 */
	UPROPERTY(Replicated)
		float MaxPlayerLife;

	/**
	 *	@property MaxPlayerStamina
	 *	@brief how much stamina this player can have
	 */
	UPROPERTY(Replicated)
		float MaxPlayerStamina;

	/**
 *	@property PlayerStaminaRegenRate
 *	@brief how fast the player can regenerate stamina
 */
	UPROPERTY(Replicated)
		float PlayerStaminaRegenRate;


public:
	// inherited via Team Interface
	FORCEINLINE FTeam I_GetTeam() const override {return PlayerTeam.GetId();}
	FORCEINLINE void I_SetTeam(FTeam NewTeam) override { PlayerTeam.SetId(NewTeam); }


	// inherited via Life Interface
	FORCEINLINE virtual float I_GetLifePoints() const override { return PlayerLife; }
	 virtual bool I_TakeDamage(const float &DamageAmount, AActor* Instigator) override;

public:
	/**
	 *	@fn GetStamina
	 *	@return the Stamina of the player
	 */
	FORCEINLINE float GetStamina() const  { return PlayerStamina; }


	 /**
	  *	@fn GetStamina_BP
	  *	@return the Stamina of the player
	  *	@note for blueprints
	  */
	UFUNCTION(BlueprintPure, Category = "Player|Stamina", meta = (DisplayName="Get Player Stamina"))
	FORCEINLINE float GetStamina_BP() const  { return PlayerStamina; }

	/**
	 *	@fn GetStamina_BP
	 *	@brief consume the player stamina
	 *	@return the Stamina left
	 */
	UFUNCTION()
		FORCEINLINE float UseStamina(float amount) { PlayerStamina -= amount;  return PlayerStamina; }

	/**
	 *	@fn GetStamina_BP
	 *	@brief consume the player stamina
	 *	@return the Stamina left
	 *	@note for blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "Player|Stamina", meta = (DisplayName = "Use Player Stamina"))
		FORCEINLINE float UseStamina_BP(float amount) {  return UseStamina(amount); }

	/**
	 *	@fn GetLife
	 *	@return the amount of life the player has
	 */
	FORCEINLINE float GetLife() const  { return I_GetLifePoints(); }

	/**
	 *	@fn GetLife
	 *	@return the amount of life the player has
	 */
	FORCEINLINE float GetMaxLife() const { return MaxPlayerLife; }

	/**
	 *	@fn GetLife_BP
	 *	@return the amount of life the player has
	 *	@note for blueprints
	 */
	UFUNCTION(BlueprintPure, Category = "Player|Stamina", meta = (DisplayName = "Get Player Life"))
	FORCEINLINE float GetLife_BP() const  { return GetLife(); }

	/**
	 *	@fn GetMaxLife_BP
	 *	@return the amount of life the player has
	 *	@note for blueprints
	 */
	UFUNCTION(BlueprintPure, Category = "Player|Stamina", meta = (DisplayName = "Get Player Max Life"))
		FORCEINLINE float GetMaxLife_BP() const { return GetMaxLife(); }

	/**
	 *	@fn GetMaxLife_BP
	 *	@return the maximum amount of stamina the player can have
	 */
	UFUNCTION()
		FORCEINLINE float GetMaxStamina() const { return MaxPlayerStamina; }
	/**
	 *	@fn GetMaxLife_BP
	 *	@return the maximum amount of stamina the player can have
	 *	@note for blueprints
	 */
	UFUNCTION(BlueprintPure, Category = "Player|Stamina", meta = (DisplayName = "Get Player Max Life"))
		FORCEINLINE float GetMaxStamina_BP() const { return GetMaxStamina(); }

private :

	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_TakeDamage(const float &DamageAmount);// class AActor* Instigator);

	UFUNCTION()
		void RegenerateStamina();

	virtual void Tick(float DeltaSeconds) override;
};