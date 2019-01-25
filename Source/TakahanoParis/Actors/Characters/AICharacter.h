// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Gameplay/CustomAIController.h"
#include "AICharacter.generated.h"

class UAIPerceptionComponent;


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIDetectionDelegate, AActor*, Actor, FAIStimulus, Stimulus);

/**
 *	@class AAICharacter
 *	@brief Base class for AI character
 *	@todo Use IGenericTeamAgentInterface or replace it all together by a Team Interface of my own
 */
UCLASS()
class TAKAHANOPARIS_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	 /** Default UObject constructor. */
	AAICharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called at Spawn or level start
	virtual void BeginPlay() override;

	// Called Every Frame
	virtual void Tick(float DeltaSeconds) override;


	/**
	 *	@property PatrolPath
	 *	@brief Path to follow
	 */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "IA")
	class ASplinePathActor * PatrolPath;

	/**
	 *	@fn	GetAIController()
	 *	@return The CustomAIController for this actor
	 */
	UFUNCTION()
		ACustomAIController * GetCustomAIController() const {return Cast<ACustomAIController>(GetController());}

	/**
	 *	@fn	GetAIController_BP()
	 *	@return The CustomAIController for this actor
	 *	@note For blueprints
	 */
	UFUNCTION(BlueprintPure, Category="AI", meta = (DisplayName = "Get Custom AI Controller"))
		ACustomAIController * GetCustomAIController_BP() const {return GetCustomAIController();}

	/**
	 *	@fn	Attack()
	 *	@brief Attack event for the Character
	 */
	UFUNCTION()
	virtual bool Attack(AActor * Target);

	/**
	 *	@fn	Attack()
	 *	@brief Attack event for the Character
	 *	@note For blueprints
	 */
	UFUNCTION(BlueprintImplementableEvent , Category = "AI|Attack", meta = (DisplayName = "Attack"))
		void Attack_BP(AActor * Target);


protected:

	/**
	 *	@property Range
	 *	@brief Attack Range of the character
	 */
	UPROPERTY(EditAnywhere, Category = "AI|Attack")
			float AttackRange;


	/**
	 *	@property Range
	 *	@brief Attack Range of the character
	 */
	UPROPERTY(EditAnywhere, Category = "AI|Attack")
		float AttackRate;

	UPROPERTY(Replicated)
		float AttackCoolDown;

	/**
	*	@property Range
	*	@brief Attack Range of the character
	*/
	UPROPERTY(EditAnywhere, Category = "AI|Attack")
		float AttackDamage =100.f;

public:

	FORCEINLINE float GetRange() const { return AttackRange; }
	FORCEINLINE float GetRate() const { return AttackRange; }

	UFUNCTION(BlueprintPure, Category = "AI|Attack", meta = (DisplayName = "Get Attack Range") )
		FORCEINLINE float GetRange_BP() const { return GetRange(); }

	UFUNCTION(BlueprintPure, Category = "AI|Attack", meta = (DisplayName = "Get Attack Rate"))
		FORCEINLINE float GetRate_BP() const { return GetRate(); }


};
