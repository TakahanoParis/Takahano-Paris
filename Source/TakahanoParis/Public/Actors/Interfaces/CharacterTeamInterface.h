// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamInterface.h"
#include "CharacterTeamInterface.generated.h"


/**
 * Uinterface class
 * This class does not need to be modified.
 */
UINTERFACE(MinimalAPI, meta =(CannotImplementInterfaceInBlueprint))
class UCharacterTeamInterface : public UTeamInterface
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
class TAKAHANOPARIS_API ICharacterTeamInterface : public ITeamInterface
{
	GENERATED_BODY()


public:

		virtual FTeam I_GetTeam() const override;

protected:

		virtual void I_SetTeam(FTeam NewTeam) override;


};
