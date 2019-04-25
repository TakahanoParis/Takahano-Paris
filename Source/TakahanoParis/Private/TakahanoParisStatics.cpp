// Fill out your copyright notice in the Description page of Project Settings.

#include "TakahanoParisStatics.h"
#include "Gameplay/CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/CustomGameState.h"
#include "GameModeInterface.h"
#include "HackInterface.h"
#include "ConstructorHelpers.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "Animation/AnimBlueprint.h"
#include "TakahanoParisSingleton.h"
#include "Components/SkeletalMeshComponent.h"
#include "UserWidget.h"
#include "WidgetBlueprintLibrary.h"

#include "UserInterface/OptionMenuWidget.h"
#include "UserInterface/MainMenuWidget.h"
#include "UserInterface/PauseMenuWidget.h"

// Binding names
FName UTakahanoParisStatics::Forward		= TEXT("Forward");
FName UTakahanoParisStatics::Right			= TEXT("Right");
FName UTakahanoParisStatics::NextCamera		= TEXT("NextCamera");
FName UTakahanoParisStatics::PreviousCamera	= TEXT("PreviousCamera");
FName UTakahanoParisStatics::Use			= TEXT("Use");
FName UTakahanoParisStatics::Sprint			= TEXT("Sprint");
FName UTakahanoParisStatics::Climb			= TEXT("Climb");
FName UTakahanoParisStatics::Jump			= TEXT("Jump");
FName UTakahanoParisStatics::PauseMenu		= TEXT("PauseGame");
FName UTakahanoParisStatics::Select			= TEXT("ClickSelect");


bool FCharacterStruct::SetSkeletalMesh(USkeletalMeshComponent* Comp) const
{
	if (!Comp || !Mesh || !AnimInstance)
		return false;
	Comp->SetSkeletalMesh(Mesh);
	Comp->SetAnimInstanceClass(AnimInstance);
	return true;

}

UCustomGameInstance* UTakahanoParisStatics::GetCustomGameInstance(const UObject* WorldContextObject)
{
	return Cast <UCustomGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}


bool UTakahanoParisStatics::EndGameToMainMenuMap(const UObject* WorldContextObject)
{
	const auto aGI = GetCustomGameInstance(WorldContextObject);
	if (!aGI)
		return false;
	aGI->ReturnToMainMenu();
	return true;
}

 void UTakahanoParisStatics::GetAllPlayerControllers(TArray<class ACustomPlayerController*> &OutControllers, const UObject* WorldContextObject)
{
	const auto  aGI = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (aGI)
		aGI->OpenLevelAndLoadSave(FName(*UGameplayStatics::GetCurrentLevelName(WorldContextObject)));
}

 void UTakahanoParisStatics::RestartGameLevel( const UObject* WorldContextObject)
 {
	 const auto  aGI = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	 if (aGI)
		 aGI->OpenLevelAndLoadSave(FName(*UGameplayStatics::GetCurrentLevelName(WorldContextObject)));

 }

EPlayStateEnum UTakahanoParisStatics::SetGameOver(const UObject* WorldContextObject, FString& GameOverMessage,
	const EGameOverEnum& GameOverReason)
{
	//UGameplayStatics::GetGameMode(WorldContextObject);
	 	GameOverMessage = TEXT("Game Over");
	switch (GameOverReason)
	{
	case EGameOverEnum::GOE_Defeat:
		RestartGameLevel(WorldContextObject);
		break;
	case EGameOverEnum::GOE_StopPlay:
		EndGameToMainMenuMap(WorldContextObject);
	default:
		break;
	}
	return EPlayStateEnum::PSE_GameOver;
}

UUserWidget* UTakahanoParisStatics::OpenMainMenu(const UObject* WorldContextObject, APlayerController* Controller)
{
	CloseMainMenu(WorldContextObject, Controller);
	bool IsValid = false;
	const auto Class = UTakahanoParisStatics::GetMainMenuWidgetClass(IsValid);
	if (!IsValid || !Class || !Controller)
		return nullptr;
	const auto MainMenu =  CreateWidget<UMainMenuWidget>(Controller, Class);
	if (!MainMenu)
		return nullptr;
	MainMenu->AddToViewport();
	return MainMenu;
}

void UTakahanoParisStatics::CloseMainMenu(const UObject* WorldContextObject, APlayerController* Controller)
{
	TArray<UUserWidget *> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(Controller, Widgets, UMainMenuWidget::StaticClass(), false);
	for (auto it : Widgets)
	{
		auto Pause = Cast<UMainMenuWidget>(it);
		if (!Pause)
			continue;
		Pause->SetIsEnabled(false);
		Pause->RemoveFromParent();
	}
}

UUserWidget* UTakahanoParisStatics::OpenOptionMenu(const UObject* WorldContextObject, APlayerController* Controller)
{
	CloseOptionMenu(WorldContextObject, Controller);
	bool IsValid = false;
	const auto Class = UTakahanoParisStatics::GetOptionMenuWidgetClass(IsValid);
	if (!IsValid || !Class || !Controller)
		return nullptr;
	const auto OptionMenu = CreateWidget<UMainMenuWidget>(Controller, Class);
	if (!OptionMenu)
		return nullptr;
	OptionMenu->AddToViewport();
	return OptionMenu;
}

void UTakahanoParisStatics::CloseOptionMenu(const UObject* WorldContextObject, APlayerController* Controller)
{
	TArray<UUserWidget *> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(Controller, Widgets, UOptionMenuWidget::StaticClass(), false);
	for (auto it : Widgets)
	{
		auto Pause = Cast<UOptionMenuWidget>(it);
		if (!Pause)
			continue;
		Pause->SetIsEnabled(false);
		Pause->RemoveFromParent();
	}
}

UUserWidget* UTakahanoParisStatics::OpenPauseMenu(const UObject* WorldContextObject, APlayerController * Controller)
{
	// Close any existing menu
	ClosePauseMenu(WorldContextObject, Controller);

	// Open a new one
	bool IsValid = false;
	const auto Class = UTakahanoParisStatics::GetPauseWidgetClass(IsValid);
		if (!IsValid)
			return nullptr;

	auto Widget = CreateWidget<UPauseMenuWidget>(Controller, Class);
	if (!Widget)
		return nullptr;
	Widget->AddToViewport();
	Widget->SetIsEnabled(true);
	return Widget;
}

void UTakahanoParisStatics::ClosePauseMenu(const UObject* WorldContextObject, APlayerController * Controller)
{
	TArray<UUserWidget *> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(Controller, Widgets,  UPauseMenuWidget::StaticClass(), false);
	for (auto it : Widgets)
	{
		auto Pause = Cast<UPauseMenuWidget>(it);
		if (!Pause)
			continue;
		Pause->SetIsEnabled(false);
		Pause->RemoveFromParent();
	}
}

void UTakahanoParisStatics::LoadLastSave(const UObject* WorldContextObject)
{
	const auto gameState = Cast<ACustomGameState>(UGameplayStatics::GetGameState(WorldContextObject));
	if (gameState)
	{
		gameState->LoadGame();
	}
}

uint8 UTakahanoParisStatics::GetDefaultAITeamID(const UObject* WorldContextObject)
{
	const auto aGM = UGameplayStatics::GetGameMode(WorldContextObject);
	if (aGM)
		return  IGameModeInterface::Execute_GetDefaultAITeamID(aGM);
	return 1;	
}

uint8 UTakahanoParisStatics::GetDefaultPlayerTeamID(const UObject* WorldContextObject)
{
	const auto aGM = UGameplayStatics::GetGameMode(WorldContextObject);
	if (aGM)
		return  IGameModeInterface::Execute_GetDefaultPlayerTeamID(aGM);
	return 255;
}

bool UTakahanoParisStatics::CallHackInterfaceOnActor(AActor * Target, class AController * Instigator)
{
	const auto AsInterface = Cast<IHackInterface>(Target);
	if (!AsInterface)
		return false;
	if (Instigator)
		IHackInterface::Execute_I_Server_Hack(Target, Instigator);
	return true;
}

bool UTakahanoParisStatics::CallInteractInterfaceOnActor(AActor* Target, AController* Instigator)
{
	const auto AsInterface = Cast<IInteractInterface>(Target);
	if (!AsInterface)
		return false;
	if (Instigator)
		IHackInterface::Execute_I_Server_Use(Target, Instigator);
	return true;
}

UTakahanoParisSingleton* UTakahanoParisStatics::GetTakahanoParisData(bool& DataIsValid)
{
	DataIsValid = false;
	UTakahanoParisSingleton* DataInstance = Cast<UTakahanoParisSingleton>(GEngine->GameSingleton);
	if (!DataInstance) return nullptr;
	if (!DataInstance->IsValidLowLevel()) return nullptr;
	DataIsValid = true;
	return DataInstance;
}

FCharacterStruct UTakahanoParisStatics::GetTakahanoParisCymie(bool& IsValid)
{
	IsValid = false;
	const auto Singleton = GetTakahanoParisData(IsValid);
	if (!IsValid)
		return FCharacterStruct();
	return Singleton->Cymie;
}

FCharacterStruct UTakahanoParisStatics::GetTakahanoParisJulia(bool& IsValid)
{
	IsValid = false;
	const auto Singleton = GetTakahanoParisData(IsValid);
	if (!IsValid)
		return FCharacterStruct();
	return Singleton->Julia;
}

FCharacterStruct UTakahanoParisStatics::GetTakahanoParisCharacterByName(const FName& CharName, bool& IsValid)
{
	IsValid = false;
	const auto Singleton = GetTakahanoParisData(IsValid);
	if (!IsValid)
		return FCharacterStruct();
	IsValid = false;
	for(auto it : Singleton->OtherCharacters)
	{
		if (it.CharaterName == CharName)
		{
			IsValid = true;
			return it;
		}
	}
	return FCharacterStruct();
}

bool UTakahanoParisStatics::SetTakahanoParisCharacter(const FCharacterStruct& Char, USkeletalMeshComponent * Comp)
{
	return Char.SetSkeletalMesh(Comp);
	//Comp->SetAnimInstanceClass(Char.AnimInstance); // Already done in SetSkeletalMesh
}

TSubclassOf<UUserWidget> UTakahanoParisStatics::GetDialogueWidgetClass(bool &bIsValid)
{
	const auto singleton = GetTakahanoParisData(bIsValid);
	if (!bIsValid)
		return UUserWidget::StaticClass();

	return singleton->DialogueWidgetClass;
}

TSubclassOf<UUserWidget> UTakahanoParisStatics::GetMainMenuWidgetClass(bool& bIsValid)
{
	const auto singleton = GetTakahanoParisData(bIsValid);
	if (!bIsValid)
		return UUserWidget::StaticClass();

	return singleton->MainMenuClass;
}

TSubclassOf<UUserWidget> UTakahanoParisStatics::GetOptionMenuWidgetClass(bool& bIsValid)
{
	const auto singleton = GetTakahanoParisData(bIsValid);
	if (!bIsValid)
		return UUserWidget::StaticClass();

	return singleton->OptionMenuClass;
}

TSubclassOf<UUserWidget> UTakahanoParisStatics::GetPauseWidgetClass(bool& bIsValid)
{
	const auto singleton = GetTakahanoParisData(bIsValid);
	if (!bIsValid)
		return UUserWidget::StaticClass();

	return singleton->PauseMenuClass;
}

UMaterialInterface* UTakahanoParisStatics::GetCursorToWorldMaterial(bool& bIsValid)
{
	const auto singleton = GetTakahanoParisData(bIsValid);
	if (!bIsValid)
		return nullptr;

	return singleton->CursorToWorldMaterial;

}

