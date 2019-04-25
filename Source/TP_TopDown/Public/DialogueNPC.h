// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DialogueNPC.generated.h"

//#include "DialoguePlugin/Public/Dialogue.h"

class ADialoguePlayerController;
class UBoxComponent;
class UDialogue;

UCLASS(hidecategories = ("Collision"))
class TP_TOPDOWN_API ADialogueNPC : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dialogue", meta = (AllowPrivateAccess = true))
		UBoxComponent * DialogueTrigger;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		UDialogue * DialogueClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		uint8 MinimumCharactersInZone;

	UPROPERTY(EditDefaultsOnly, blueprintReadOnly, Category = "Dialogue")
		bool  bCanTalk;

public:

	UPROPERTY(EditAnywhere, blueprintReadOnly, Category = "Dialogue")
		FVector  BoxExtent;

	UFUNCTION(BlueprintPure, Category = "Dialogue")
		UDialogue * GetNPCDialogue() const { return DialogueClass; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
		bool DialogueConditionFulfilled();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void DialogueOccured(APlayerController * Player, bool bCannotTalkAgain = true);

private :
	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnBoxEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UFUNCTION()
		virtual void OnPlayerEntered(APlayerController *Player);

	UFUNCTION()
		virtual void OnPlayerLeft(APlayerController *Player);

private:

	UPROPERTY()
		TArray<ACharacter *> PlayersInZone;

	UPROPERTY()
		bool EndMainDialogueLine;

public :

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		 void MainDialogueOver(bool bIsDone = true) {EndMainDialogueLine = bIsDone;}

	UFUNCTION(BlueprintPure, Category = "Dialogue")
		FORCEINLINE bool GetMainDialogueIsOver() const { return EndMainDialogueLine;}
};
