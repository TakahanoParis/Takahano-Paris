

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

	UFUNCTION(BlueprintCallable, Category = "Level")
		void RestartGameLevel();

	UFUNCTION(BlueprintCallable, Category = "PlayState")
		void SetGameOver();

	UFUNCTION(BlueprintCallable, Category = "PlayState")
		void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "PlayState")
		void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "PlayState")
		void SetCanStartMatch();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bCanStartMatch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EPlayStateEnum PlayState;


protected:

	UPROPERTY()
		int8 PlayerControllerNum;

	UPROPERTY(EditDefaultsOnly)
		int8 MaxPlayerNumber;

	virtual void PostLogin(APlayerController * NewPlayer) override;

	virtual void PreLogin(const FString & Options, const FString & Address, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage) override;

	virtual void Logout(AController * Exiting) override;


	virtual bool ReadyToStartMatch_Implementation() override;

#if 0
	//team
public:

	UFUNCTION(BlueprintPure)
		FTeam GetTeam(int TeamNumber);

	UFUNCTION(BlueprintPure)
		bool TeamExists(int TeamNumber);


protected:
	UPROPERTY()
		TArray<FTeam> TeamLists;

	UPROPERTY()
		int NumOfTeam;
#endif

public:
	FORCEINLINE class FTimerManager * GetTimerManager() { return  &MultiPlayerTimerManager; }
private:
	class FTimerManager MultiPlayerTimerManager;
};


