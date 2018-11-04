// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Actors/Interfaces/TeamInterface.h"
#include "CustomPlayerState.generated.h"

/**
 *	@class ACustomPlayerState
 *	@brief player infos : Life points, stamina, etc...
 *	@note all the handling of the info could be handled by the character but the default design of UE4 is to have this separate class
 */
UCLASS()
class TAKAHANOPARIS_API ACustomPlayerState : public APlayerState, public ITeamInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated)
		FTeam HeroTeam;
public:
	FORCEINLINE FTeam I_GetTeam() const override {return HeroTeam.GetId();}
	FORCEINLINE void I_SetTeam(FTeam NewTeam) override { HeroTeam.SetId(NewTeam); }

	
};