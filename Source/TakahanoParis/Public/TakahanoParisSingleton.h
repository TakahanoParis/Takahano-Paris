// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TakahanoParisStatics.h"
#include "DialogueUserWidget.h"
#include "TakahanoParisSingleton.generated.h"



class USkeletalMesh;
class UAnimBlueprintGeneratedClass;
class USkeletalMeshComponent;


/**
 *  Class Holding all defaults value to use in TakahanoParis
 */
UCLASS(Blueprintable, BlueprintType)
class TAKAHANOPARIS_API UTakahanoParisSingleton : public UObject
{
	GENERATED_BODY()

public :

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
		FCharacterStruct Cymie;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
		FCharacterStruct Julia;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
		TArray<FCharacterStruct> OtherCharacters;

	

	/**	Class to use for creating a Main menu - allows of using a class defined in Blueprint */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UUserWidget> MainMenuClass;

	/**	Class to use for creating a Option menu - allows of using a class defined in Blueprint */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UUserWidget> OptionMenuClass;

	/**	Class to use for creating a Main menu - allows of using a class defined in Blueprint */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Dialogue")
		TSubclassOf<UDialogueUserWidget> DialogueWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor")
		class UMaterialInterface * CursorToWorldMaterial;

};
