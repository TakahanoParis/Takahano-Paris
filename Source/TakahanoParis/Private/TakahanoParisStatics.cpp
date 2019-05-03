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
#include "TakahanoParis.h"
#include "UserInterface/OptionMenuWidget.h"
#include "UserInterface/MainMenuWidget.h"
#include "UserInterface/PauseMenuWidget.h"
#include "GameFramework/InputSettings.h"
#include "UserInterface/MainHUDWidget.h"

// Binding names
FName UTakahanoParisStatics::Forward			= TEXT("Forward");
FName UTakahanoParisStatics::Right				= TEXT("Right");
FName UTakahanoParisStatics::NextTarget			= TEXT("NextCamera");
FName UTakahanoParisStatics::PreviousTarget		= TEXT("PreviousCamera");
FName UTakahanoParisStatics::Use				= TEXT("Activate");
FName UTakahanoParisStatics::Sprint				= TEXT("Sprint");
FName UTakahanoParisStatics::Climb				= TEXT("Climb");
FName UTakahanoParisStatics::Jump				= TEXT("Jump");
FName UTakahanoParisStatics::PauseMenu			= TEXT("PauseGame");
FName UTakahanoParisStatics::Select				= TEXT("ClickSelect");
FName UTakahanoParisStatics::Dialogue			= TEXT("Dialogue");
FName UTakahanoParisStatics::CameraXAxis		= TEXT("Turn");
FName UTakahanoParisStatics::CameraYAxis		= TEXT("LookUp");
FName UTakahanoParisStatics::CameraXAxisRate	= TEXT("TurnRate");
FName UTakahanoParisStatics::CameraYAxisRate	= TEXT("LookUpRate");
FName UTakahanoParisStatics::Cancel				= TEXT("Cancel");

void FInputBindingInfo::SetFromSettings()
{
	const auto Actions = FindActionsInSettings(BindingName);
	if(Actions.Num() > 0)
	{
		for (auto it : Actions)
			ActionIcons.Add(FInputIcon(it));
		return;
	}
	const auto Axis = FindAxisInSettings(BindingName);
	{
		for (auto it : Axis)
			ActionIcons.Add(FInputIcon(it.Key));
		return;
	}
}

TArray<FKey> FInputBindingInfo::FindActionsInSettings(const FName& Name) const
{
	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return TArray<FKey>();
	const TArray<FInputActionKeyMapping>& Actions = Settings->ActionMappings;
	TArray<FKey> AssociatedKeys;
	for (auto Action : Actions)
		if (Action.ActionName == Name) AssociatedKeys.Add(Action.Key);
	return AssociatedKeys;
}

TArray<TPair<FKey, float>> FInputBindingInfo::FindAxisInSettings(const FName& Name) const
{
	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return TArray<TPair<FKey, float>>();
	const TArray<FInputAxisKeyMapping>& Axis = Settings->AxisMappings;
	TArray<TPair<FKey, float>> AssociatedAxis;
	for (auto Axe : Axis)
		if (Axe.AxisName == Name) AssociatedAxis.Add(TPair<FKey,float>(Axe.Key, Axe.Scale));
	return AssociatedAxis; //always separate axis and Actions
}

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

void UTakahanoParisStatics::SaveGame(const UObject* WorldContextObject)
{
	const auto gameState = Cast<ACustomGameState>(UGameplayStatics::GetGameState(WorldContextObject));
	if (gameState)
	{
		gameState->SaveGame();
	}
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

UUserWidget* UTakahanoParisStatics::CreateMainHUDWidget(const UObject* WorldContextObject,
	APlayerController* Controller)
{
	if (!Controller)
		Controller = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	// Close any existing menu
	DeleteMainHUDWidget(WorldContextObject, Controller);

	// Open a new one
	bool IsValid = false;
	const auto Class = UTakahanoParisStatics::GetMainHUDWidgetClass(IsValid);
	if (!IsValid)
		return nullptr;
	if (!Class)
	{
#if WITH_EDITOR
		UE_LOG(LogTakahanoParis, Error, TEXT("UTakahanoParisStatics,CreateMainHUDWidget : No class in singleton "));
#endif //WITH_EDITOR
		return nullptr;
	}
	auto Widget = CreateWidget<UMainHUDWidget>(Controller, Class);
	if (!Widget)
		return nullptr;
	Widget->AddToViewport();
	Widget->SetIsEnabled(true);

	Widget->SetHelp(FText(), false);
	Widget->SetTarget(FName(), nullptr, false);

	return Widget;
}

UUserWidget* UTakahanoParisStatics::GetMainHUDWidget(const UObject* WorldContextObject, APlayerController* Controller)
{
	if (!Controller)
		Controller = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	TArray<UUserWidget *> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(Controller, Widgets, UMainHUDWidget::StaticClass(), false);

	if (Widgets.Num() > 1)
	{
		for (int32 c = 1 ; c < Widgets.Num(); c++)
		{
			if(!Widgets.IsValidIndex(c))
				break;

			auto HUD = Cast<UMainHUDWidget>(Widgets[c]);
			if (!HUD)
				continue;
			HUD->SetIsEnabled(false);
			HUD->RemoveFromParent();
		}
	}
	return Widgets.IsValidIndex(0) ? Widgets[0]: nullptr;
}

void UTakahanoParisStatics::ToggleVisibilityMainHUDWidget(const UObject* WorldContextObject, bool Visibility,
	APlayerController* Controller)
{
	if (!Controller)
		Controller = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	TArray<UUserWidget *> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(Controller, Widgets, UMainHUDWidget::StaticClass(), false);
	for (auto it : Widgets)
	{
		auto HUD = Cast<UMainHUDWidget>(it);
		if (!HUD)
			continue;
		HUD->SetVisibility(Visibility ? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	}
}

void UTakahanoParisStatics::DeleteMainHUDWidget(const UObject* WorldContextObject,
	APlayerController* Controller)
{
	if (!Controller)
		Controller = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	TArray<UUserWidget *> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(Controller, Widgets, UMainHUDWidget::StaticClass(), false);
	for (auto it : Widgets)
	{
		auto HUD  = Cast<UMainHUDWidget>(it);
		if (!HUD)
			continue;
		HUD->SetIsEnabled(false);
		HUD->RemoveFromParent();
	}
}

void UTakahanoParisStatics::SetPlayerTarget(const UObject* WorldContextObject, const FName &TargetName, const AActor* Target, APlayerController * Controller)
{
	if (!WorldContextObject)
		return;
	const auto HUD = Cast<UMainHUDWidget>(GetMainHUDWidget(WorldContextObject, Controller));
	if (!HUD)
		return;
	HUD->SetTarget(TargetName, Target, Target ? true : false);
}

void UTakahanoParisStatics::SetPlayerHelpByBinding(const UObject* WorldContextObject, const FName &BindingName, bool Enable , APlayerController * Controller)
{
	const auto HUD = Cast<UMainHUDWidget>(GetMainHUDWidget(WorldContextObject, Controller));
	if (!HUD)
		return;
	HUD->SetHelpByBinding(BindingName, Enable);
}

void UTakahanoParisStatics::SetPlayerHelp(const UObject* WorldContextObject, const FText& Message, bool Enable,
	APlayerController* Controller)
{
	const auto HUD = Cast<UMainHUDWidget>(GetMainHUDWidget(WorldContextObject, Controller));
	if (!HUD)
		return;
	HUD->SetHelp(Message, Enable);
}

void UTakahanoParisStatics::SetPlayerObjective(const UObject* WorldContextObject, const FText& ObjectiveText, bool Enable,
	APlayerController* Controller)
{
	const auto HUD = Cast<UMainHUDWidget>(GetMainHUDWidget(WorldContextObject, Controller));
	if (!HUD)
		return;
	HUD->SetObjective(ObjectiveText, Enable);
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
	if (!GEngine) return nullptr;
	if (!GEngine->GameSingleton) return nullptr;
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

TSubclassOf<UUserWidget> UTakahanoParisStatics::GetMainHUDWidgetClass (bool& bIsValid)
{
	const auto singleton = GetTakahanoParisData(bIsValid);
	if (!bIsValid)
		return UUserWidget::StaticClass();

	return singleton->MainHUDClass;
}

UMaterialInterface* UTakahanoParisStatics::GetCursorToWorldMaterial(bool& bIsValid)
{
	const auto singleton = GetTakahanoParisData(bIsValid);
	if (!bIsValid)
		return nullptr;

	return singleton->CursorToWorldMaterial;

}

FText UTakahanoParisStatics::GetDialogueHelpText()
{
	bool IsValid;
	const auto singleton = GetTakahanoParisData(IsValid);
	if (!IsValid || !singleton)
		return FText();

	const auto SingletonText = singleton->DialogueHelpText;;

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return SingletonText;
	const TArray<FInputActionKeyMapping>  Keys = Settings->ActionMappings.FilterByPredicate([](const FInputActionKeyMapping &In) { return In.ActionName == UTakahanoParisStatics::Dialogue; });
	FString Help = TEXT("Press ");
	for (auto it : Keys)
	{
		Help.Append(it.Key.GetDisplayName().ToString());
		Help.Append(TEXT(" "));
	}
	Help.Append(SingletonText.ToString());
	return FText::FromString(Help);
}

FText UTakahanoParisStatics::GetUseHelpText()
{
	bool IsValid;
	const auto singleton = GetTakahanoParisData(IsValid);
	if (!IsValid || !singleton)
		return FText();

	const auto SingletonText = singleton->UseHelpText;;

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return SingletonText;
	const TArray<FInputActionKeyMapping>  Keys = Settings->ActionMappings.FilterByPredicate([](const FInputActionKeyMapping &In) { return In.ActionName == UTakahanoParisStatics::Use; });
	FString Help = TEXT("Press ");
	for (auto it : Keys)
	{
		Help.Append(it.Key.GetDisplayName().ToString());
		Help.Append(TEXT(" "));
	}
	Help.Append(SingletonText.ToString());
	return FText::FromString(Help);
}

FText UTakahanoParisStatics::GetPreviousTargetHelpText()
{
	bool IsValid;
	const auto singleton = GetTakahanoParisData(IsValid);
	if (!IsValid || !singleton)
		return FText();

	const auto SingletonText = singleton->PreviousTargetHelpText;;

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return SingletonText;
	const TArray<FInputActionKeyMapping>  Keys = Settings->ActionMappings.FilterByPredicate([](const FInputActionKeyMapping &In) { return In.ActionName == UTakahanoParisStatics::PreviousTarget; });
	FString Help = TEXT("Press ");
	for (auto it : Keys)
	{
		Help.Append(it.Key.GetDisplayName().ToString());
		Help.Append(TEXT(" "));
	}
	Help.Append(SingletonText.ToString());
	return FText::FromString(Help);
}

FText UTakahanoParisStatics::GetNextTargetHelpText()
{
	bool IsValid;
	const auto singleton = GetTakahanoParisData(IsValid);
	if (!IsValid || !singleton)
		return FText();

	const auto SingletonText = singleton->NextTargetHelpText;;

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return SingletonText;
	const TArray<FInputActionKeyMapping>  Keys = Settings->ActionMappings.FilterByPredicate([](const FInputActionKeyMapping &In) { return In.ActionName == UTakahanoParisStatics::NextTarget; });
	FString Help = TEXT("Press ");
	for (auto it : Keys)
	{
		Help.Append(it.Key.GetDisplayName().ToString());
		Help.Append(TEXT(" "));
	}
	Help.Append(SingletonText.ToString());
	return FText::FromString(Help);
}

/*
TArray<FInputBindingInfo> UTakahanoParisStatics::GetSettingsInputBindingInfos()
{
	TArray<FInputBindingInfo> Array;
	Array.Add(FInputBindingInfo(Forward));
	Array.Add(FInputBindingInfo(Right));
	Array.Add(FInputBindingInfo(NextTarget));
	Array.Add(FInputBindingInfo(PreviousTarget));
	Array.Add(FInputBindingInfo(Use));
	Array.Add(FInputBindingInfo(Sprint));
	Array.Add(FInputBindingInfo(Climb));
	Array.Add(FInputBindingInfo(Jump));
	Array.Add(FInputBindingInfo(PauseMenu));
	Array.Add(FInputBindingInfo(Select));
	Array.Add(FInputBindingInfo(Dialogue));
	return Array;
}
TArray<FInputBindingInfo> UTakahanoParisStatics::GetAllInputBindingInfos(bool& bIsValid)
{
	const auto singleton = GetTakahanoParisData(bIsValid);
	if (!bIsValid)
		return GetSettingsInputBindingInfos();
	return singleton->InputBindingInfos;
}

FText UTakahanoParisStatics::GetBindingHelpText(FName Binding, bool& bIsValid)
{
	const auto BInfo = GetAllInputBindingInfos(bIsValid);
	if (!bIsValid)
		return FText();
	const auto InfoPtr = BInfo.FindByPredicate([Binding](const FInputBindingInfo &InInfo)
	{
		return InInfo.GetName() == Binding;
	});
	if (InfoPtr)
		return InfoPtr->GetInfo();
	return FText();
}
*/

