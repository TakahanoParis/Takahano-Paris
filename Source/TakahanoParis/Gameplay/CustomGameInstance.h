// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UnrealNetwork.h"
#include "Online.h"
#include "StreamingPauseRendering.h"
#include "CustomGameInstance.generated.h"



USTRUCT(Blueprintable)
struct FServerSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int MaxPlayer;

	UPROPERTY(VisibleAnywhere)
		int CurrentPlayers;
	
	UPROPERTY(EditAnywhere)
		bool bIsLan;

	UPROPERTY(EditAnywhere)
		FString ServerName; // You may wanna make this a FName or less likely a Ftext

	//UPROPERTY() // Cannot be UPROPERTY, will not be replicated
	//	FOnlineSessionSettings * SessionSettings;
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	FServerSettings()
	{
		MaxPlayer = 0;
		CurrentPlayers = 0;
		ServerName = "ErrorServerNotSet";
	}
};

/**
 * @class UCustomGameInstance
 * Base class of Game instance in Takahano Paris
 * Handles all the networking and the in's and out of the game
 */
UCLASS(ClassGroup = (Gameplay))
class TAKAHANOPARIS_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public :
	UCustomGameInstance(const FObjectInitializer& ObjectInitializer);


protected:

	/**
	 *	@brief HeroClasses Properties
 	 *	List the heroes implemented in this version of the game
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Heros")
		TArray<TSubclassOf<class AHero>> HeroClasses;


public:

	/**
	 *	@brief HeroClasses Properties
	 *	allow accessing the List of heroes implemented in this version of the game
	 */
	UFUNCTION()
		FORCEINLINE TArray<TSubclassOf<class AHero>> GetAllHeroes() const { return HeroClasses; }

	/**
	 *	@brief HeroClasses Properties
	 *	allow accessing the List of heroes implemented in this version of the game
	 *	@note for Bluerint use, do not call in C++
	 */
	UFUNCTION(BlueprintPure, Category = "Heroes", meta = (DisplayName = "Get all Implemented Heroes"))
		FORCEINLINE TArray<TSubclassOf<class AHero>> GetAllHeroes_BP() const { return GetAllHeroes(); }



public:

	/**
	* @brief ShowMainMenu Function
	* Shows the main menu, for local Player controller
	*/
	UFUNCTION()
		virtual void ShowMainMenu();


	/**
	* @brief ShowHostMenu Function
	* Shows the Host menu, for local Player controller
	*/
	UFUNCTION()
		virtual void ShowHostMenu();

	/**
	* @brief ShowServerMenu Function
	* Shows the Host menu, for local Player controller
	*/
	UFUNCTION()
		virtual void ShowServerMenu();

	/**
	* @brief ShowOptionMenu Function
	* Shows the Host menu, for local Player controller
	*/
	UFUNCTION()
		virtual void ShowOptionMenu();

	/**
	* @brief ShowQuitGameConfirmationMessage Function
	* Shows A message asking for confirmation of Quit command, for local Player controller
	*/
	UFUNCTION()
		virtual void ShowQuitGameConfirmationMessage();

	/**
	* @brief QuitGame Function
	* Quit the programm, for local Player controller
	* @todo should be protected
	*/
	UFUNCTION()
		void QuitGame();

	/**
	* @brief HostGame Function
	* Host A game with default Online SubSystem
	*/
	UFUNCTION()
		virtual void HostGame();

	/**
	* @brief RequestHostGame Function
	* Request Host A game with default Online SubSystem
	*/
	UFUNCTION()
		virtual void RequestHostGame();



protected:

	/**	
	 * @brief CreateMainMenu Function
	 * Spawns a main menu if necessary (none exists)
	 * @return true if it created a menu or one ealready existed.
	 */
	UFUNCTION()
		virtual bool CreateMainMenu();

	/**
	* @brief CreateHostMenu Function
	* Spawns a Host menu if necessary (none exists)
	* @return  true if it created a menu or one ealready existed.
	*/
	UFUNCTION()
		virtual bool CreateHostMenu();

	/**
	* @brief CreateServerMenu Function
	* Spawns a Server menu if necessary (none exists)
	* @return  true if it created a menu or one ealready existed.
	*/
	UFUNCTION()
		virtual bool CreateServerMenu();

	/**
	* @brief CreateOptionMenu Function
	* Spawns a Option menu if necessary (none exists)
	* @return  true if it created a menu or one ealready existed.
	*/
	UFUNCTION()
		virtual bool CreateOptionMenu();


	UPROPERTY()
	class UMainMenuWidget * MainMenu;
	
	UPROPERTY()
	class UOptionMenuWidget * OptionMenu;


	/**	Class to use for creating a Main menu - allows of using a class defined in Blueprint */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSubclassOf<class UMainMenuWidget> MainMenuClass;

	/**	Class to use for creating a Option menu - allows of using a class defined in Blueprint */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSubclassOf<class UOptionMenuWidget> OptionMenuClass;
	
	
private:

	UPROPERTY(Replicated)
		FServerSettings NetSettings;

public:

	/**
	 * @brief LaunchLobby Function
	 * Opens A Lobby
	 */
	UFUNCTION()
		void LaunchLobby(FServerSettings NewServerSettings);

	/**
	* @brief LaunchLobby Function
	* Opens A Lobby
	*/
	UFUNCTION()
		void FindSessions(bool bIsLan);

	/**
	* @brief LaunchLobby Function
	* Opens A Lobby
	*/
	UFUNCTION()
		void JoinSessions(FName SessionToJoin);

	UFUNCTION()
		void DestroySessionAndLeaveGame();

	/**
	 * @brief ShowLoadingScreen Function
	 * Shows the LoadingScreen, for local Player controller
	 * Use that to Hide some time consuming operations
	 */
	UFUNCTION()
		virtual void ShowLoadingScreen();

	/**
	* @brief ShowPleaseWaitMessage Function
	* Shows a Please Wait, for local Player controller
	* Use that to Hide some time consuming operations, while not hiding all interface
	*/
	UFUNCTION()
		virtual void ShowPleaseWaitMessage();


	/*
	UFUNCTION()
		virtual void DestroyPendingLoadingScreen();
	*/ 

	UFUNCTION()
		virtual void DestroyPendingPleaseWaitMessage();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maps and Modes")
		TSoftObjectPtr<UWorld> MainMenuUMap;
	FName MainMenuMapName;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maps and Modes")
		TSoftObjectPtr<UWorld> LobbyUMap;
	FName LobbyMapName;


protected:
	/** Called when the game instance is started either normally or through PIE. */
	virtual void OnStart() override;

	virtual void ReturnToMainMenu() override;


private:

	//FStreamingPauseRenderingModule PauseModule;


	/// Session Hosting Finding and Joining backend based on https://wiki.unrealengine.com/How_To_Use_Sessions_In_C%2B%2B
private:
	/// Hosting sessions

	/**
	 *	@brief HostSession Function
	 *	Backend Function to create a Session
	 *
	 *	@Param		UserID			User that started the request
	 *	@Param		SessionName		Name of the Session
 	 *	@Param		bIsLAN			Is this is LAN Game?
	 *	@Param		bIsPresence		"Is the Session to create a presence Session"
	 *	@Param		MaxNumPlayers	        Number of Maximum allowed players on this "Session" (Server)
	 *
	 *	@return true if successful
	 */
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	/**
	*	Function fired when a session create request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);


	/// Finding Sessions

	/**
	*	Find an online session
	*
	*	@param UserId user that initiated the request
	*	@param bIsLAN are we searching LAN matches
	*	@param bIsPresence are we searching presence sessions
	*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);


	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	/**	A variable for our SearchSettings which will also contain our SearchResults, once this search is complete and successful:*/
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/**
	*	Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);


	/// Joining Sessions

	/**
	*	@brief JoinMultiplayerSession - JoinSession is already a UGameInstanceFunction
	*	Joins a session via a search result
	*
	*	@param SessionName name of session
	*	@param SearchResult Session to join
	*
	*	@return bool true if successful, false otherwise
	*/
	bool JoinMultiplayerSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/**
	*	Delegate fired when a session join request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


	/// Destroying Session
	/** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/**
	*	Delegate fired when a destroying an online session has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

};
