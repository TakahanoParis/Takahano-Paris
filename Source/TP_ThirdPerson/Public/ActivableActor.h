// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActivableActor.generated.h"


class AThirdPersonCharacter;

/**
 *	ACameraActor class  
 *	An actor you can use to see the world through
 */
UCLASS(ClassGroup = (Actor))
class TP_THIRDPERSON_API AActivableActor: public AActor
{
	GENERATED_BODY()

public:

	/**
	 * @brief Default Constructor
	 */
	AActivableActor();

	/**
	* @brief CanUse() Check if this actor is usable
	* @return true if it is usable
	*/
	UFUNCTION(BlueprintPure, Category = "Use")
	virtual bool CanUse(AThirdPersonCharacter * UserCharacter);

protected:
	/**
	* @brief Have you got the right to use this ?
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activable")
		bool bIsUsable;

	/**
	* @brief Have you got the right to use this ?
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activable")
		float MinimumUseDistance;


	/**
	* @brief Are you one of those that can use it (should be a sanity check)
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activable")
		TArray<TSubclassOf<AThirdPersonCharacter>> AuthorizedClass;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Activable")
		void Use(AThirdPersonCharacter * PlayerCharacter, bool &bShouldLockMovement);
	virtual void Use_Implementation(AThirdPersonCharacter * PlayerCharacter, bool &bShouldLockMovement);

	UFUNCTION(BlueprintPure, Category = "Activable")
		virtual FName GetUsableName() const;




};

