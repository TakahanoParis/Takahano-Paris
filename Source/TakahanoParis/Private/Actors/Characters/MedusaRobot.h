// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/AICharacter.h"
#include "MedusaRobot.generated.h"

/**
 * 
 */
UCLASS()
class AMedusaRobot : public AAICharacter
{
	GENERATED_BODY()


public :
	AMedusaRobot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medusa")
		int Group;

	UFUNCTION()
		void SetAIEnable(bool bEnable = true);

	UFUNCTION(BlueprintCallable, Category = "AI|Medusa", meta =(DisplayName = "Enable"))
		void SetAIEnable_BP(bool bEnable = false) { SetAIEnable(bEnable); }


	virtual bool CanRun() override;
};
