// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gameplay/CustomAIController.h"
#include "SaveableActorInterface.h"
#include "LifeInterface.h"
#include "CharacterTeamInterface.h"
#include "AICharacter.generated.h"


class UAIPerceptionComponent;


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIDetectionDelegate, AActor*, Actor, FAIStimulus, Stimulus);

/**
 *	@class AAICharacter
 *	@brief Base class for AI character
 *	@todo Use IGenericTeamAgentInterface or replace it all together by a Team Interface of my own
 */
UCLASS(ClassGroup = (Character), config = Game)
class TAKAHANOPARIS_API AAICharacter : public ACharacter, public ICharacterTeamInterface, public ILifeInterface, public ISaveableActorInterface
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
	UFUNCTION(BlueprintPure, Category = "AI", meta = (DisplayName = "Get Custom AI Controller"))
		ACustomAIController * GetCustomAIController() const {return Cast<ACustomAIController>(GetController());}


	/**
	 *	@fn	Attack()
	 *	@brief Attack event for the Character
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "AI|Attack", meta = (DisplayName = "Attack"))
		void Attack(AActor * Target);

	/**
	*	@fn	StopAttack()
	*	@brief Stop Attack event for the Character
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "AI|Attack", meta = (DisplayName = "Stop Attack"))
		void StopAttack(AActor * Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
		void Run();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
		void StopRunning();

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

	UFUNCTION(BlueprintPure, Category = "AI|Attack", meta = (DisplayName = "Get Attack Range"))
		float GetRange() const { return AttackRange; }

	UFUNCTION(BlueprintPure, Category = "AI|Attack", meta = (DisplayName = "Get Attack Rate"))
		float GetRate() const { return AttackRange; }

public:

	float I_GetLifePoints() const override { return LifePoints; }
	bool I_TakeDamage(const float& DamageAmount, AActor* Instigator) override;

private:

	float LifePoints;

};
