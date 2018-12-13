

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CustomGameMode.generated.h"

/**
* @enum EPlayStateEnum represents the State of the Game
*/
UENUM(BlueprintType)		
enum class EPlayStateEnum : uint8
{
	PSE_Play		UMETA(DisplayName = "Play"),
	PSE_Pause		UMETA(DisplayName = "Pause"),
	PSE_GameOver	UMETA(DisplayName = "Game Over"),
};

/**
* @enum EPlayStateEnum represents the State of the Game
*/
UENUM(BlueprintType)
enum class EGameOverEnum : uint8
{
	GOE_Victory		UMETA(DisplayName = "Victory"),
	GOE_Defeat		UMETA(DisplayName = "Defeat")
};



/**
 * @class This is the Base GameMode for all TakahanoParis Game.
 * @note GameMode Only exists on Server.
 */
UCLASS()
class ACustomGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACustomGameMode(const FObjectInitializer& ObjectInitializer);

public:

	/**
	*	@fn GetAllPlayerControllers
	*	@brief Tries to mqke an Array of all valid player controllers
	*	@param OutControllers : Output parameter made of all valid Controllers
	*/
	UFUNCTION()
		void GetAllPlayerControllers(TArray<class ACustomPlayerController*> &OutControllers);

	/**
	*	@fn GetAllPlayerControllers
	*	@brief Tries to mqke an Array of all valid player controllers
	*	@param OutControllers : Output parameter made of all valid Controllers
	*	@return false if encountered error, true otherwise
	*	@note for Blueprints
	*/
	UFUNCTION(BlueprintPure, Category = "Players", meta =(DisplayName = "Get All Player controllers" )  )
		void GetAllPlayerControllers_BP(TArray<class ACustomPlayerController*> &OutControllers) { GetAllPlayerControllers(OutControllers); }


	/**
	 *	@fn RestartGameLevel
	 *	@brief restart level
	 */
	UFUNCTION()
		void RestartGameLevel();

	/**
	 *	@fn RestartGameLevel
	 *	@brief restart level
	 *	@note For Blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "Level", meta = (DisplayName = "Restart Level"))
		void RestartGameLevel_BP() { RestartGameLevel(); }


	/**
	 *	@fn SetGameOver
	 *	@brief End Level and call the related event
	 *	@note For Blueprints
	 */
	UFUNCTION()
		virtual void SetGameOver(FString &GameOverMessage, const TEnumAsByte<EGameOverEnum> &GameOverReason);

	/**
	 *	@fn SetGameOver
	 *	@brief End Level and call the related event
	 *	@note For Blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayState", meta = (DisplayName = "Game Over"))
		void SetGameOver_BP(FString &GameOverMessage, const TEnumAsByte<EGameOverEnum> GameOverReason){ SetGameOver(GameOverMessage,GameOverReason); }


	/**
	 *	@fn PauseGame
	 *	@brief Pause Game
	 */
	UFUNCTION()
		virtual void PauseGame();

	/**
	 *	@fn PauseGame
	 *	@brief Pause Game
	 *	@note For Blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayState", meta = (DisplayName = "Pause Match"))
		void PauseGame_BP() { PauseGame(); }

	/**
	 *	@fn PauseGame
	 *	@brief resume game if the game was paused 
	 */
	UFUNCTION()
		void ResumeGame();

	/**
	 *	@fn PauseGame
	 *	@brief resume game if the game was paused
	 *	@note For Blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayState", meta = (DisplayName = "Resume Match"))
		void ResumeGame_BP() { ResumeGame(); }


	/**
	 *	@fn SetCanStartMatch
	 *	@brief Enable the match to start
	 */
	UFUNCTION()
		virtual void SetCanStartMatch();

	/**
	 *	@fn SetCanStartMatch
	 *	@brief Enable the match to start
	 *	@note For Blueprints
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayState", meta = (DisplayName = "Set Can Start Match"))
		void SetCanStartMatch_BP() { SetCanStartMatch(); }

protected:


	/**
	 *	@property bCanStartMatch
	 *	@brief describe if the match is ready to start
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayState")
		bool bCanStartMatch;

	/**
	 *	@property PlayState
	 *	@brief describe at which state the game is (ie. playing, paused, waiting to start)
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayState")
		EPlayStateEnum PlayState{};

	/**
	 *	@property DefaultPlayerTeamID
	 *	@brief The Team ID for the new real players
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Teams")
		uint8 DefaultPlayerTeamID;

	/**
	 *	@property DefaultIATeamID
	 *	@brief The Team ID for the new IA players
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Teams")
		uint8 DefaultAITeamID;



public :

	/**
	 *	@fn GetDefaultPlayerTeamID
	 *	@brief Getter for the DefaultPlayerTeamID
	 *	@return The Team ID for the new IA players
	 */
	UFUNCTION(BlueprintPure, Category = "Teams", meta = (DisplayName = "Get Default AI Team ID"))
		FORCEINLINE uint8 GetDefaultPlayerTeamID() const { return DefaultPlayerTeamID; };

	/**
	 *	@fn GetDefaultPlayerTeamID_BP
	 *	@brief Getter for the DefaultPlayerTeamID
	 *	@return The Team ID for the new IA players
	 *	@note for Blueprints
	 */
	UFUNCTION(BlueprintPure, Category = "Teams", meta = (DisplayName = "Get Default AI Team ID"))
		FORCEINLINE int GetDefaultPlayerTeamID_BP() const { return GetDefaultPlayerTeamID(); };


	/**
	 *	@fn GetDefaultAITeamID
	 *	@brief Getter for the DefaultAITeamID
	 *	@return The Team ID for the new IA players
	 */
	UFUNCTION(BlueprintPure, Category = "Teams", meta = (DisplayName = "Get Default AI Team ID"))
		FORCEINLINE uint8 GetDefaultAITeamID() const { return DefaultAITeamID; };

	/**
	 *	@fn GetDefaultIATeamID_BP
	 *	@brief Getter for the DefaultAITeamID
	 *	@return The Team ID for the new IA players
	 *	@note for Blueprints
	 */
	UFUNCTION(BlueprintPure, Category = "Teams", meta= (DisplayName = "Get Default AI Team ID"))
		FORCEINLINE int GetDefaultAITeamID_BP() const { return GetDefaultAITeamID(); };

protected:

	/**
	 *	@property PlayerControllerNum
	 *	@brief The number of player connected to the game
	 */
	UPROPERTY()
		int8 PlayerControllerNum;

	/**
	 *	@property PlayerControllerNum
	 *	@brief The maximum of players connected to the game
	 */
	UPROPERTY(EditDefaultsOnly)
		int8 MaxPlayerNumber;

	/**
	 *	@fn PostLogin
	 *	@brief what to do when a player joins
	 *	@param NewPlayer : the player that just joined in
	 */
	virtual void PostLogin(APlayerController * NewPlayer) override;

	/**
	 *	@fn PreLogin
	 *	@brief what to do when a player tries to join
	 *	@param Options the option set from connection
	 *	@param UniqueId the ID of the incomming connection
	 *	@param ErrorMessage the error message that we will send in case there's an error
	 */
	virtual void PreLogin(const FString & Options, const FString & Address, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage) override;

	/**
	 *	@fn Logout
	 *	@brief what to do when a player tries to join
	 *	@param Exiting the player controller that's leaving
	 */
	virtual void Logout(AController * Exiting) override;

	/**
	 *	@fn ReadyToStartMatch_Implementation
	 *	@brief called to check if we can start the match or no
	 *	@return true if the match can start, false otherwise
	 */
	virtual bool ReadyToStartMatch_Implementation() override;


private:

	UFUNCTION()
	/**
	 *	@fn EndGameToMainMenuMap()
	 *	@brief Calls game instance to open Main Menu Map
	 *	@return true if it successfully called game instance function, false otherwise
	 */
		bool EndGameToMainMenuMap();
public:
	FORCEINLINE class FTimerManager * GetTimerManager() { return  &MultiPlayerTimerManager; }
private:
	class FTimerManager MultiPlayerTimerManager;


public :
	virtual class AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override { return false; }
};


