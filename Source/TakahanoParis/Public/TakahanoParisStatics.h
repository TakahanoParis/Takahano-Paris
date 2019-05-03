// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "Engine/InputAxisKeyDelegateBinding.h"
#include "TakahanoParisStatics.generated.h"

//#include "TakahanoParisSingleton.h"

//struct FKey;
class UTexture2D;
class UCustomGameInstance;
class UTakahanoParisStatics;
class USkeletalMesh;
class UAnimInstance;
class UTakahanoParisSingleton;
class UUserWidget;



USTRUCT(BlueprintType, meta = (DisplayName = "Input Icon"))
struct FInputIcon
{
	GENERATED_BODY()

protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D * Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FKey Key;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsGamepad{};
public :

	FInputIcon(FKey InputKey = FKey()) : Key(InputKey), Icon(nullptr)
	{
	}
};

USTRUCT(BlueprintType, meta = (DisplayName = "Input Info"))
struct FInputBindingInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName BindingName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Help Text"))
		FText ActionInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FInputIcon> ActionIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsSituational;

public:
	FInputBindingInfo(FName Name = FName("NoName"), FText Info = FText(), bool Situational = false,
	                  const TArray<FInputIcon>& Icons = TArray<FInputIcon>()) : BindingName(Name), ActionInfo(Info),
	                                                                            ActionIcons(Icons),
	                                                                            bIsSituational(Situational)
	{
		SetFromSettings();
	}

	void SetFromSettings();

	FORCEINLINE void SetInfo(const FText &info) { ActionInfo = info; }
	FORCEINLINE void SetIcons(const TArray<FInputIcon> &icons) {ActionIcons = icons;	}
	FORCEINLINE void SetIsSituational(const bool &situational) {bIsSituational	= situational; }

	FORCEINLINE FName GetName() const { return BindingName; }
	FORCEINLINE FText GetInfo() const { return ActionInfo; }
	FORCEINLINE TArray<FInputIcon> GetIcons() const  { return ActionIcons; }
	FORCEINLINE bool GetIsSituational() const { return bIsSituational; }

private:
	TArray<FKey> FindActionsInSettings(const FName &Name) const;
	TArray<TPair<FKey,float>> FindAxisInSettings(const FName &Name) const;
};




USTRUCT(BlueprintType, meta = (DisplayName= "Character Data"))
struct FCharacterStruct
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USkeletalMesh * Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimBlueprintGeneratedClass * AnimInstance{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName CharaterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName ExtendedName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText Description;


private:

	bool bIsValid{};

public:
	FCharacterStruct(FName NewName = TEXT("Name Not Invalid"), USkeletalMesh* NewMesh = nullptr,
	                 UAnimBlueprintGeneratedClass* NewAnim = nullptr) : CharaterName(NewName), Mesh(NewMesh),
	                                                                    AnimInstance(NewAnim)
	{
		bIsValid = (Mesh && AnimInstance);
	}

	bool SetSkeletalMesh(USkeletalMeshComponent * Comp) const;

	bool IsValid()
	{
		bIsValid = (Mesh && AnimInstance);
		return bIsValid;
	}

private:

	friend UTakahanoParisSingleton;
	friend UTakahanoParisStatics;
};

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
	GOE_Defeat		UMETA(DisplayName = "Defeat"),
	GOE_StopPlay	UMETA(DisplayName = "End Of Play")
};



/**
 * 
 */
UCLASS()
class TAKAHANOPARIS_API UTakahanoParisStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	*	@fn GetCustomGameInstance()
	*	@brief Retrieve a pointer to the Game instance if it is a CustomGameInstance
	*/
	UFUNCTION(BlueprintPure, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
		static UCustomGameInstance* GetCustomGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext= "WorldContextObject"))
		static bool EndGameToMainMenuMap(const UObject* WorldContextObject);

	/**
	*	@fn GetAllPlayerControllers
	*	@brief Tries to mqke an Array of all valid player controllers
	*	@param OutControllers : Output parameter made of all valid Controllers
	*/
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
		static void GetAllPlayerControllers(TArray<class ACustomPlayerController*> &OutControllers, const UObject* WorldContextObject);
	
	/**
	*	@fn RestartGameLevel
	*	@brief restart level
	*/
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
	static void RestartGameLevel(const UObject* WorldContextObject);


	/**
	*	@fn SetGameOver
	*	@brief End Level and call the related event
	*	@note For Blueprints
	*/
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|GameState", meta = (WorldContext = "WorldContextObject"))
		static EPlayStateEnum SetGameOver(const UObject* WorldContextObject, FString &GameOverMessage, const EGameOverEnum &GameOverReason);


	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|GameState|Save", meta = (WorldContext = "WorldContextObject"))
		static void SaveGame(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|GameState|Save", meta = (WorldContext = "WorldContextObject"))
		static void LoadLastSave(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
		static uint8 GetDefaultAITeamID(const UObject * WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
		static uint8 GetDefaultPlayerTeamID(const UObject * WorldContextObject);

	/**	This is a way to call interface function for other modules	*/
	UFUNCTION(BlueprintCallable)
		static bool CallHackInterfaceOnActor(AActor * Target, class AController * Instigator);

	/**	This is a way to call interface function for other modules	*/
	UFUNCTION(BlueprintCallable)
		static bool CallInteractInterfaceOnActor(AActor * Target, class AController * Instigator);

	// Singleton				-----------------------------------------------------------------------------------------

	/**	Get SingletonData anywhere, anytime	*/
	UFUNCTION(BlueprintPure, Category = "Takahano-Paris|Singleton")
		static UTakahanoParisSingleton* GetTakahanoParisData(bool &DataIsValid);

	// Characters				-----------------------------------------------------------------------------------------

	/**	Cymie, From Singleton	*/
	UFUNCTION(BlueprintPure, Category = "Takahano-Paris|Character")
		static FCharacterStruct GetTakahanoParisCymie(bool &IsValid);

	/**	Julia, From Singleton	*/
	UFUNCTION(BlueprintPure, Category = "Takahano-Paris|Character")
		static FCharacterStruct GetTakahanoParisJulia(bool &IsValid);

	/**	Any Character, From Singleton	*/
	UFUNCTION(BlueprintPure, Category = "Takahano-Paris|Character")
		static FCharacterStruct GetTakahanoParisCharacterByName(const FName &CharName, bool &IsValid);

	/**	Setup any character From a defined struct	*/
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|Character")
		static bool SetTakahanoParisCharacter(const FCharacterStruct &Char, USkeletalMeshComponent * Comp);


	// UI				-----------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI|Dialogue")
		static TSubclassOf<UUserWidget> GetDialogueWidgetClass(bool &bIsValid);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI")
		static TSubclassOf<UUserWidget> GetMainMenuWidgetClass(bool &bIsValid);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI")
		static TSubclassOf<UUserWidget> GetOptionMenuWidgetClass(bool &bIsValid);

	UFUNCTION(BlueprintCallable, Category ="Takahano-Paris|UI")
		static TSubclassOf<UUserWidget> GetPauseWidgetClass(bool &bIsValid);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI|HUD")
		static TSubclassOf<UUserWidget> GetMainHUDWidgetClass(bool &bIsValid);


	/**
	* @brief OpenMainMenu()
	* Spawns a main menu if necessary (none exists)
	* @return  true if it created a menu or one ealready existed.
	*/
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI", meta = (WorldContext = "WorldContextObject"))
		static UUserWidget * OpenMainMenu(const UObject* WorldContextObject, APlayerController * Controller);

	/**
	* @brief CreateOptionMenu()
	* Closes all instances of  Main Menus
	*/
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI", meta = (WorldContext = "WorldContextObject"))
		static void CloseMainMenu(const UObject* WorldContextObject, APlayerController * Controller);


	/**
	* @brief OpenOptionMenu()
	* Spawns an Option menu if necessary (none exists)
	* @return  true if it created a menu or one ealready existed.
	*/
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI", meta = (WorldContext = "WorldContextObject"))
		static UUserWidget * OpenOptionMenu(const UObject* WorldContextObject, APlayerController * Controller);

	/**
	* @brief CloseOptionMenu()
	* Closes all instances of  Option Menus
	*/
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI", meta = (WorldContext = "WorldContextObject"))
		static void CloseOptionMenu(const UObject* WorldContextObject, APlayerController * Controller);

	/**
	*	@brief OpenPauseMenu()
	*	Open the pause Menu during Gameplay
	*/
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI", meta = (WorldContext = "WorldContextObject"))
		static UUserWidget * OpenPauseMenu(const UObject* WorldContextObject, APlayerController * Controller);

	/**
	*	@brief ClosePauseMenu()
	*	Close all instances of Pause Menues
	*/
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI", meta = (WorldContext = "WorldContextObject"))
		static void ClosePauseMenu(const UObject* WorldContextObject, APlayerController * Controller);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI|HUD", meta = (WorldContext = "WorldContextObject"))
		static UUserWidget * CreateMainHUDWidget(const UObject* WorldContextObject, APlayerController * Controller = nullptr);

	UFUNCTION(BlueprintPure, Category = "Takahano-Paris|UI|HUD", meta = (WorldContext = "WorldContextObject"))
		static UUserWidget * GetMainHUDWidget(const UObject* WorldContextObject, APlayerController * Controller = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI|HUD", meta = (WorldContext = "WorldContextObject"))
		static void ToggleVisibilityMainHUDWidget(const UObject* WorldContextObject,bool Visibility  = false,  APlayerController * Controller = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|UI|HUD", meta = (WorldContext = "WorldContextObject"))
		static void DeleteMainHUDWidget(const UObject* WorldContextObject, APlayerController * Controller = nullptr);

	// Effects Info & More	-----------------------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
		static void SetPlayerTarget(const UObject* WorldContextObject, const FName &TargetName, const AActor* Target, APlayerController * Controller = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
		static void SetPlayerHelpByBinding(const UObject* WorldContextObject, const FName &BindingName, bool Enable = true,  APlayerController * Controller = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
		static void SetPlayerHelp(const UObject* WorldContextObject, const FText &Message, bool Enable = true, APlayerController * Controller = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
		static void SetPlayerObjective(const UObject* WorldContextObject, const FText &ObjectiveText, bool Enable = true, APlayerController * Controller = nullptr);

	// Cursor				-----------------------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Cursor")
		static class UMaterialInterface  * GetCursorToWorldMaterial(bool &bIsValid);


	// Input Bindings		-----------------------------------------------------------------------------------------
	static FName Forward;
	static FName Right;
	static FName NextTarget;
	static FName PreviousTarget;
	static FName Use;
	static FName Sprint;
	static FName Climb;
	static FName Jump;
	static FName PauseMenu;
	static FName Select;
	static FName Dialogue;
	static FName CameraXAxis;
	static FName CameraYAxis;
	static FName CameraXAxisRate;
	static FName CameraYAxisRate;
	static FName Cancel;
	/*
	UFUNCTION(BlueprintPure, Category = "Binding")
	static TArray<FInputBindingInfo> GetSettingsInputBindingInfos();

	UFUNCTION(BlueprintPure, Category = "Binding")
		static TArray<FInputBindingInfo> GetAllInputBindingInfos(bool &bIsValid);

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FText GetBindingHelpText(FName Binding, bool &bIsValid);
*/


	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetForwardBinding() { return Forward; }

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetRightBinding() { return Right; }

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetNextTargetBinding() { return NextTarget; }

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetPreviousTargetBinding() { return PreviousTarget; }

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetUseBinding() { return Use; }

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetSprintBinding() { return Sprint; }

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetClimbBinding() { return Climb; }

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetJumpBinding() { return Jump; }

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetPauseBinding() { return PauseMenu; }

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetSelectBinding() { return Select; }

	UFUNCTION(BlueprintPure, Category = "Binding")
		static FName GetDialogueBinding() { return Dialogue; }

	UFUNCTION(BlueprintPure, Category = "Help")
		static FText GetDialogueHelpText();

	UFUNCTION(BlueprintPure, Category = "Help")
		static FText GetUseHelpText();

	UFUNCTION(BlueprintPure, Category = "Help")
		static FText GetPreviousTargetHelpText();

	UFUNCTION(BlueprintPure, Category = "Help")
		static FText GetNextTargetHelpText();


};
