// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ability.generated.h"
#include "TimerManager.h"

/**
 * @enum ECoolDownState
 * 		 Represents all the possible states for the CoolDown Timer
 */
UENUM()
enum class ECoolDownState : uint8
{
    CDS_Ready 		UMETA(DisplayName ="Ready"),
    CDS_Paused 		UMETA(DisplayName ="Paused"),
    CDS_Running 	UMETA(DisplayName ="Running")

};

/**
 * @Class Ability
 *  	  Default class for using a passive or active ability.
 */
UCLASS(ClassGroup = (Hero) )
class TAKAHANOPARIS_API UAbility : public UActorComponent
{
GENERATED_BODY()

public:
// Sets default values for this component's properties
UAbility();

protected:
    /** 
     *	@property CoolDown
     *	@brief Cooldown time for the ability used to setup the timer 
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Cooldown", meta = (DisplayName = "Cooldown"))
    float CoolDown;

    /** 
     *	@property Rate
     *	@brief Rate for the timer`, defines how fast the cooldown gets removed 
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category= "Cooldown", meta = (DisplayName = "Cooldown Rate"))
    float Rate;

    /**
     *	@property bIsPassive 
     *	@brief Boolean Flag to set the ability as passive - ie. will act on it's own and does not require the user to trigger it
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Is Passive ?"))
    bool bIsPassive = false;

    /** 
     *	@property Level 
     *	@brief int value describing the level of the ability (if it can be upgraded)
     */
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, category= "Cooldown", meta = (DisplayName = "Cooldown Rate"))
    int Level;

    UFUNCTION(BlueprintCallable)
            void IncrementLevel();
	
    /**
     * @fn SetTimer()
     * Allow for setting the timer to start with rate and time set to the class variables
     * @param Default - set to false to use the other parameters
     * @param NewCoolDown - Replacement CoolDown time to use instead of class property
     * @param NewRate - Replacement Rate to use instead of class property
     * @see CoolDown
     * @see Rate
     */
    UFUNCTION()
    void SetTimer(bool Default = true, float NewCoolDown = 0.f, float NewRate = 0.f);


    /**
    * @brief PauseTimer Function
    * Allow for pausing the timer
    * @see CoolDown
    * @see Rate
    */
    UFUNCTION()
    void PauseTimer();

    /**
    * @brief PauseTimer Function
    * Allow for pausing the timer
    * @see CoolDown
    * @see Rate
    */
    UFUNCTION()
    void UnPauseTimer();

public:

    /**
    * @fn GetCoolDown()
    * @return the amount of time left before the ability can be used, returns -2.f on Clients
    */
    UFUNCTION(BlueprintPure, Category = "Use")
    float GetCoolDown();

    /**
     *	@fn TryUse()
     *	@brief Check if the Ability can be used and if so use it.
     *	@return true if Use() has been called
     */
    UFUNCTION(BlueprintCallable , Category = "Use")
    bool TryUse();


protected:


    /**	Called at PlayerSpawn, Used for starting the cooldown*/
    virtual void BeginPlay() override;

    /** Called post Constructor, before BeginPlay */
    virtual void InitializeComponent() override;

    /**
     * @brief function used to Get the Correct TimerManager.
     * @return A Time Manager or nullptr if none was found
     */
    virtual class FTimerManager *  GetTimerManager();

    UFUNCTION(blueprintPure)
    TEnumAsByte < enum ENetRole >   GetOwnerRole() const;

    UFUNCTION(blueprintPure)
    ECoolDownState GetTimerState() const;

    /**
     * @brief Use function
     * Activate the effect of the Ability - even passive abilities have a Use function, as that is what activate the effect
    */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Use"))
    virtual void Use();



    /** 
     *	@fn SetReady()
     *	@brief Callback Function for the CoolDownState - Set to CDS_Ready
     */
    UFUNCTION()
    virtual void SetReady();

    /** 
     *	@fn SetPaused()
     *	@brief Callback Function for the CoolDownState - Set to CDS_Paused
     */
    UFUNCTION()
    virtual void SetPaused();
    /** 
     *	@fn SetRunning()
     *	@brief Callback Function for the CoolDownState - Set to CDS_Running
     */
    UFUNCTION()
    virtual void SetRunning();



private:

    /**
     *	@property bCanUse
     *	@brief Boolean flag to allow the use of the ability 
     */
    UPROPERTY(ReplicatedUsing = OnRep_CanUse)
    bool bCanUse;


    /** 
     *	@property CDState
     *	@brief This represents the current state of the Timer
     */
    UPROPERTY(ReplicatedUsing = OnRep_CoolDownState)
    ECoolDownState CDState;

    /**
     * @property CoolDownDelegate
     * @brief property to access the Timer function for the cooldown
     * This could be replicated, but since there are 5 abilities,
     * and at as much as 10 players this could lead to network stress
     * instead we just synchronise  with Server authority when trying to use.
     * @see CDState;
     * @see TryUse();
     * @See Use();
     */
    FTimerDelegate	CoolDownDelegate;
	/**
	 * @property CoolDownHandle
	 * @See CoolDownDelegate;
	 */
    FTimerHandle	CoolDownHandle;

    /**
     *	@fn Server_ResetCoolDown()
     *	@brief Resets the CoolDown timer on Server.
     *	@see ResetCoolDown()
     */
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_ResetCoolDown();

	/**
	 *	@fn OnRep_CanUse()
	 *	@brief Called when bCanUse Changes
	 */
    UFUNCTION()
    void OnRep_CanUse();

	/**
	 *	@fn OnRep_CoolDownState()
	 *	@brief Called when CoolDownState Changes
	 */
    UFUNCTION()
    void OnRep_CoolDownState();

    /**
    * @fn EndPlay()
    * @brief Get rid of the timer and some other cleanups
    */
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	/**
	 *	@fn Server_IncrementLevel()
	 *	@brief add 1 to the level of the ability. will then be replicated to all other players
	 */
    UFUNCTION(Server, Reliable, WithValidation)
        void Server_IncrementLevel();

protected:

	/**
	 *	@fn Server_IncrementLevel()
	 *	@brief add 1 to the level of the ability. will then be replicated to all other players
	 */
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Use();
    virtual void Server_Use_Implementation();

    /**
     * @fn Effect Blueprint event
     * @deprecated for development Test purposes. Will probably gets removed later on.
     */
    UFUNCTION(BlueprintImplementableEvent, Category="Effect")
    void BlueprintEffect();

};
