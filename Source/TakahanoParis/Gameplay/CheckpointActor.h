// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CheckpointActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCheckpointEnteredDelegate);

class Actor;

/**
 * 
 */
UCLASS()
class TAKAHANOPARIS_API ACheckpointActor : public ATriggerBox
{
	GENERATED_BODY()

public:

	ACheckpointActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


#if 0
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Checkpoint")
		int NumberOfPlayerRequiered;
#endif //0

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly, Category = "")
	TArray<AActor* > ActorsInside;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "")
		TArray<APlayerController* > PlayerPassed;

	/**
	 *	@fn OnBeginOverlap
	 *	@brief On End  overlapped
	 *	@param OverlappedActor
	 *	@param OtherActor
	 */
	UFUNCTION()
		virtual void OnBeginOverlap(AActor* OverlappedActor, AActor * OtherActor);


	/**
	 *	@fn OnEndOverlap
	 *	@brief On End  overlapped 
	 *	@param OverlappedActor
	 *	@param OtherActor
	 */
	UFUNCTION()
		virtual void OnEndOverlap(AActor* OverlappedActor, AActor * OtherActor);

	UPROPERTY()
		FCheckpointEnteredDelegate OnCheckpointEntered;

	/**
	 *	@fn OnPlayeredEntered
	 *	@brief Called when a Player enters the Checkpoint
	 *	@note Blueprint Native Event
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Checkpoint")
		void OnPlayeredEntered();




	
	
	
	
};
