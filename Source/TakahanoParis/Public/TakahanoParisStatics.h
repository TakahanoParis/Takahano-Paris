// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "TakahanoParisStatics.generated.h"
//#include "TakahanoParisSingleton.h"


class UTakahanoParisStatics;
class USkeletalMesh;
class UAnimInstance;
class UTakahanoParisSingleton;


USTRUCT(BlueprintType, meta = (DisplayName= "Character Data"))
struct FCharacterStruct
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USkeletalMesh * Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimBlueprintGeneratedClass * AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName CharaterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName ExtendedName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText Description;

	UPROPERTY()

private:

	bool bIsValid;

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
	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
		static EPlayStateEnum SetGameOver(const UObject* WorldContextObject, FString &GameOverMessage, const EGameOverEnum &GameOverReason);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris", meta = (WorldContext = "WorldContextObject"))
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

	UFUNCTION(BlueprintPure, Category = "Takahano-Paris")
		static UTakahanoParisSingleton* GetTakahanoParisData(bool &DataIsValid);

	UFUNCTION(BlueprintPure, Category = "Takahano-Paris|Character")
		static FCharacterStruct GetTakahanoParisCymie(bool &IsValid);

	UFUNCTION(BlueprintPure, Category = "Takahano-Paris|Character")
		static FCharacterStruct GetTakahanoParisJulia(bool &IsValid);

	UFUNCTION(BlueprintPure, Category = "Takahano-Paris|Character")
		static FCharacterStruct GetTakahanoParisCharacterByName(const FName &CharName, bool &IsValid);

	UFUNCTION(BlueprintCallable, Category = "Takahano-Paris|Character")
		static void SetTakahanoParisCharacter(const FCharacterStruct &Char, USkeletalMeshComponent * Comp);
};
