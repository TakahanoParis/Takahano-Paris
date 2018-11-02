 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


/**
* @Class The base class of all heroes and controller character of TakahanoParis.
*/
UCLASS(ClassGroup = (Character))//config=Game)
class TAKAHANOPARIS_API ABaseCharacter : public ACharacter
{
    GENERATED_BODY()


    public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseLookUpRate;

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void SetReady(bool NewReady);

    UFUNCTION(BlueprintCallable, Category ="Gameplay")
    bool GetIsReady() const;

    private:

    UPROPERTY(Replicated) //ReplicatedUsing = OnRep_IsReady)
    bool bIsReady;

    UFUNCTION()
    void OnRep_IsReady();

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_IsReady(bool ServerIsReady);

    /*
    UFUNCTION(NetMulticast, BlueprintCallable, Category = "Replication")
        void
    */
    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    /**
     * Called via input to turn at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);



    /** Called for Main Attack ability Cannot be passive	 */
    UFUNCTION()
    virtual void Attack();

	/**
	 *	@brief Ability Function 
	 *	Will try to throw the ability if it exist
	 *	@return false if it did nothing
	 */
	UFUNCTION()
	virtual bool Ability(const uint8 &Number);


    protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // End of APawn interface




#if 0
    /** Team Interface */
protected:


    virtual uint8 I_GetTeam() override;
    virtual void I_SetTeam(uint8 NewTeam) override;
#endif

};
