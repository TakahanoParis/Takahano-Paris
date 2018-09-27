// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameMapsSettings.h"
#include "UserInterface/MainMenuWidget.h"
#include "UserInterface/OptionMenuWidget.h"


UCustomGameInstance::UCustomGameInstance(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	/** Bind function for CREATING a Session */
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UCustomGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UCustomGameInstance::OnStartOnlineGameComplete);

	/** Bind function for FINDING a Session */
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UCustomGameInstance::OnFindSessionsComplete);

	/** Bind function for JOINING a Session */
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UCustomGameInstance::OnJoinSessionComplete);

	// Map names setup
	MainMenuMapName = *MainMenuUMap.GetLongPackageName();
	LobbyMapName = *LobbyUMap.GetLongPackageName();



}


///
///	Menu Functions
///

void UCustomGameInstance::ShowMainMenu()
{
	if (!CreateMainMenu())
	{
		UE_LOG(LogSlate, Error, TEXT("Couldn't Create a Main Menu"));
		return;
	}
	//APlayerController * PC = UGameplayStatics::GetPlayerController(this, 0);
	//MainMenu->AddToViewport(0);
}

void UCustomGameInstance::ShowHostMenu()
{
	CreateHostMenu();
	APlayerController * PC = UGameplayStatics::GetPlayerController(this, 0);
}

void UCustomGameInstance::ShowServerMenu()
{
	CreateServerMenu();
	APlayerController * PC = UGameplayStatics::GetPlayerController(this, 0);
}

void UCustomGameInstance::ShowOptionMenu()
{
	if (!CreateOptionMenu())
		return;
	//APlayerController * PC = UGameplayStatics::GetPlayerController(this, 0);
	//OptionMenu->AddToViewport(0);

}

void UCustomGameInstance::ShowQuitGameConfirmationMessage()
{
	UE_LOG(LogTemp, Warning, TEXT("QuitRequested"));
	//@todo : replacethat by a real message of confirmation ;)
	QuitGame();
}

void UCustomGameInstance::QuitGame()
{
#if UE_EDITOR
	UKismetSystemLibrary::QuitGame(GetWorld(), GetPrimaryPlayerController(), EQuitPreference::Type::Quit);
	return;
#endif // UE_EDITOR
	UKismetSystemLibrary::QuitGame(GetWorld(), GetPrimaryPlayerController(), EQuitPreference::Type::Quit);
	//FGenericPlatformMisc::RequestExit(false);
}

void UCustomGameInstance::HostGame()
{
	// Creating a local player where we can get the UserID from
	ULocalPlayer* const Player = GetFirstGamePlayer();
	ShowLoadingScreen();
	// Let's make sure the name is right before trying to get to Lobby
	LobbyMapName = *LobbyUMap.GetLongPackageName();
	// Call our custom HostSession function. GameSessionName is a GameInstance variable
	HostSession(Player->GetCachedUniqueNetId().GetUniqueNetId(), GameSessionName, true, true, 4);
}



void UCustomGameInstance::RequestHostGame()
{
	// For now just host with default Options
	HostGame();
}

bool UCustomGameInstance::CreateMainMenu()
{
	if (MainMenu)
		return true;

	if(!GetWorld())
		UE_LOG(LogLevel, Error, TEXT("GetWorld failed"));

	APlayerController * PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (!PC)
	{
		UE_LOG(LogSlate, Error, TEXT("No Valid Player Controller"));
		return false;
	}
	if (MainMenuClass)
		MainMenu = CreateWidget<UMainMenuWidget>(PC, MainMenuClass);

	if (MainMenu)
		return true;

	UE_LOG(LogSlate, Error, TEXT("CreateWidget Failed"));
		return false;
}

bool UCustomGameInstance::CreateHostMenu()
{
	return false;
}


bool UCustomGameInstance::CreateServerMenu()
{
	return false;
}


bool UCustomGameInstance::CreateOptionMenu()
{
	if (OptionMenu)
		return true;

	APlayerController * PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
		return false;

	if (OptionMenuClass)
		OptionMenu = CreateWidget<UOptionMenuWidget>(PC, OptionMenuClass);

	if (OptionMenu)
		return true;

	return false;
}

///
///	End Of Menu Section
///

void UCustomGameInstance::LaunchLobby(FServerSettings NewServerSettings)
{
	NetSettings = NewServerSettings;
	ShowLoadingScreen();
	// Create Session
	ULocalPlayer* const Player = GetFirstGamePlayer();
	HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, true, true, 4);

}

void UCustomGameInstance::FindSessions(bool bIsLan)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), bIsLan, true);

}

void UCustomGameInstance::JoinSessions(FName SessionToJoin)
{

}

void UCustomGameInstance::DestroySessionAndLeaveGame()
{
}

void UCustomGameInstance::ShowLoadingScreen()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	//if(Player)
	// PauseModule.BeginStreamingPause(Player->ViewportClient->Viewport);

}

void UCustomGameInstance::ShowPleaseWaitMessage()
{
}

void UCustomGameInstance::DestroyPendingPleaseWaitMessage()
{
}

void UCustomGameInstance::OnStart()
{
	// Always call super function, even if it does nothing, in case some day it does something
	Super::OnStart();
	
	//@todo Load Loading Screen ASAP

	ReturnToMainMenu();
}

void UCustomGameInstance::ReturnToMainMenu()
{
	Super::ReturnToMainMenu();
	const UGameMapsSettings* GameMapsSettings = GetDefault<UGameMapsSettings>();
	const FString& DefaultMap = GameMapsSettings->GetGameDefaultMap();
	MainMenuMapName = *MainMenuUMap.GetLongPackageName();
	if (DefaultMap != MainMenuMapName.ToString()) // Sanity check
	{
		UE_LOG(LogLoad, Warning, TEXT("DefaultMap (%s) is not the same as the Specified MainMenu Map (%s)"), *DefaultMap, *MainMenuMapName.ToString());
	}
	if (MainMenuMapName != FName())
		UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName, true, FString());
	
	//Since we're on Main Menu let's show it: PlayerControllerShoud Do it All by itself calling Show MainMenu
	//ShowMainMenu();
}

///
///	Backend Function for connecting
///

bool UCustomGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	// Get the Online Subsystem to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	if (OnlineSub)
	{
		// Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
			Fill in all the Session Settings that we want to use.

			There are more with SessionSettings.Set(...);
			For example the Map or the GameMode/Type.
			*/
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME,LobbyMapName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Let's make a Copy for later use, if we need to;
			NetSettings.SessionSettings = SessionSettings;

			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		UE_LOG(LogNet, Error,TEXT("No OnlineSubsytem found!"));
	}
	return false;
}

void UCustomGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{

	if (bWasSuccessful) {
		UE_LOG(LogNet, Display, TEXT("OnCreateSessionComplete %s Succesful"), *SessionName.ToString());
	}
	else
		UE_LOG(LogNet, Error, TEXT("OnCreateSessionComplete %s failed"), *SessionName.ToString());

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);
			}
		}
	}
}

void UCustomGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		UE_LOG(LogNet, Display, TEXT("OnStartSessionComplete %s Succesful"), *SessionName.ToString());
	}
	else
		UE_LOG(LogNet, Error, TEXT("OnStartSessionComplete %s failed"), *SessionName.ToString());
	
	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LobbyMapName, true, "listen");
	}

}

void UCustomGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
			Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			// We only want to set this Query Setting if "bIsPresence" is true
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}

void UCustomGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful) {
		UE_LOG(LogNet, Display, TEXT("OnFindSessionsComplete: Find SuccessFul"));
	}
	else
		UE_LOG(LogNet, Error, TEXT("OnFindSessionsComplete: Find failed"));

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface of the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			UE_LOG(LogNet, Display, TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num());

			// If we have found at least 1 session, we just going to debug them.
			if (SessionSearch->SearchResults.Num() > 0)
			{
				// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
				// This can be customized later on with your own classes to add more information that can be set and displayed
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
					// This is something you can't do in Blueprint for example!
					UE_LOG(LogNet, Display, TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
				}
			}
		}
	}

}

bool UCustomGameInstance::JoinMultiplayerSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult & SearchResult)
{

	// Return bool
	bool bSuccessful = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}
	return bSuccessful;
}

void UCustomGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	UE_LOG(LogNet, Warning, TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			APlayerController * const PlayerController = GetFirstLocalPlayerController();

			// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
			// String for us by giving him the SessionName and an empty String. We want to do this, because
			// Every OnlineSubsystem uses different TravelURLs
			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				// Finally call the ClienTravel. If you want, you could print the TravelURL to see
				// how it really looks like
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UCustomGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		UE_LOG(LogNet, Display, TEXT("OnDestroySessionComplete %s Succesful"), *SessionName.ToString());
	}
	else
		UE_LOG(LogNet, Error, TEXT("OnDestroySessionComplete %s failed"), *SessionName.ToString());

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			// If it was successful, we just load another level (could be a MainMenu!)
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName, true);
			}
		}
	}
}


///
/// Replication of UPROPERTYs
///
void UCustomGameInstance::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCustomGameInstance, NetSettings);

}




