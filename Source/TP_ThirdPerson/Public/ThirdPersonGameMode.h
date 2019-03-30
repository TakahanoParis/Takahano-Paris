

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameModeInterface.h"
//#include "ThirdPersonHero.h"
#include "ThirdPersonGameMode.generated.h"

class AThirdPersonHero;

/**
 * @class This is the Base GameMode for all TakahanoParis Game.
 * @note GameMode Only exists on Server.
 */
UCLASS(minimalAPI)
class AThirdPersonGameMode : public AGameMode , public IGameModeInterface
{
	GENERATED_BODY()

public:
	AThirdPersonGameMode(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Classes)
		TArray<TSubclassOf<AThirdPersonHero>> HeroClasses;



protected:


	/**
	 *	@property DefaultPlayerTeamID
	 *	@brief The Team ID for the new real players
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Teams")
		uint8 DefaultPlayerTeamID = 255;

	/**
	 *	@property DefaultIATeamID
	 *	@brief The Team ID for the new IA players
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Teams")
		uint8 DefaultAITeamID = 1;

	UClass * GetDefaultPawnClassForController_Implementation(AController* InController) override;

public :

	/**
	 *	@fn GetDefaultPlayerTeamID
	 *	@brief Getter for the DefaultPlayerTeamID
	 *	@return The Team ID for the new IA players
	 */
	uint8 GetDefaultPlayerTeamID_Implementation() const override { return DefaultPlayerTeamID; };

	/**
	 *	@fn GetDefaultAITeamID
	 *	@brief Getter for the DefaultAITeamID
	 *	@return The Team ID for the new IA players
	 */

	uint8 GetDefaultAITeamID_Implementation() const override { return DefaultAITeamID; };

};


