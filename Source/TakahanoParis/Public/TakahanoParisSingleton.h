// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TakahanoParisStatics.h"
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


	
	
};
