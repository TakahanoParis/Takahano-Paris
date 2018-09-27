
#include "Ability.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "TimerManager.h"
#include "Gameplay/CustomGameMode.h"

// Sets default values for this component's properties
UAbility::UAbility()
{
    PrimaryComponentTick.bCanEverTick = false;
    bReplicates = true;

    //CoolDownDelegate.BindUFunction(this, FName("SetReady"))

    CoolDown = 10.f;
    Rate = 1.f;



}

void UAbility::IncrementLevel()
{

}

void UAbility::BeginPlay()
{

    //UE_LOG(LogTemp, Warning, TEXT("This is executed on Server only"));
    CoolDownDelegate.BindUObject(this,&UAbility::SetReady);
    SetTimer(false); // Set the timer to 0.f

    UE_LOG(LogTemp, Error, TEXT("toto"));
    if(GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Error, TEXT("Timer exist on Server : %s"), GetTimerManager()->TimerExists(CoolDownHandle) ? TEXT("True") : TEXT("False"));
    }

}

void UAbility::InitializeComponent()
{

}

TEnumAsByte < enum ENetRole >   UAbility::GetOwnerRole() const
{
    return GetOwner()->Role;
}

ECoolDownState UAbility::GetTimerState() const
{
    return CDState;
}

FTimerManager *  UAbility::GetTimerManager()
{
    // this would try to use GameMode Timer
    /*
    AGameModeBase * GameMode = GetOwner()->GetWorld()->GetAuthGameMode();
    ACustomGameMode * AnimaGameMode = Cast<ACustomGameMode>(GameMode);
    if(AnimaGameMode)
        return AnimaGameMode->GetTimerManager();
    return nullptr;
     */
    return &(GetOwner()->GetWorld()->GetTimerManager());
}


void UAbility::SetTimer(bool Default, float NewCoolDown, float NewRate)
{
    GetTimerManager()->ClearTimer(CoolDownHandle);
    // Loop is set to "false" because we will reset the countdown after use
    GetTimerManager()->SetTimer(CoolDownHandle, CoolDownDelegate, Default?Rate:NewRate, false, Default?CoolDown:NewCoolDown);
    SetRunning();
}

void UAbility::PauseTimer()
{
    GetTimerManager()->PauseTimer(CoolDownHandle);
    SetPaused();
}

void UAbility::UnPauseTimer()
{
    GetTimerManager()->UnPauseTimer(CoolDownHandle);
    SetRunning();
}

float UAbility::GetCoolDown()
{
    // Only Return the server timer
    if(GetOwner()->HasAuthority())
    {
        if(GetTimerManager())
            return GetTimerManager()->GetTimerRemaining(CoolDownHandle);
    }
    return -2.f;
}

bool UAbility::TryUse()
{
    if(CDState != ECoolDownState::CDS_Ready)
    {
        return false;
    }
    Use();
    return true;
}



void UAbility::Use()
{
    //	//Only play the timer and do nothing else, Implementations will be in child classes
    if (GetOwnerRole() == ROLE_AutonomousProxy)
    {
        Server_Use();
        Server_ResetCoolDown();
    }
    else if(GetOwnerRole() == ROLE_Authority)
    {
        Server_Use_Implementation(); // We're already server, no need for confirmation
        Server_ResetCoolDown_Implementation();
    }
    else
        SetTimer(); // And by the way, Go F**k yourselves, I do not know who you are !
}



void UAbility::SetReady()
{
    CDState = ECoolDownState::CDS_Ready;
}
void UAbility::SetPaused()
{
    CDState = ECoolDownState::CDS_Paused;
}
void UAbility::SetRunning()
{
    CDState = ECoolDownState::CDS_Running;
}

bool UAbility::Server_Use_Validate()
{
    //Just Check CDState
    return CDState == ECoolDownState::CDS_Ready;
}
void UAbility::Server_Use_Implementation()
{
    // Here we don't do anything but call the Blueprint capabilities
    BlueprintEffect();
}

bool UAbility::Server_ResetCoolDown_Validate()
{
    // Safety mesures can be implemented here, but this should not be really a hacker target
    return true;
}

void UAbility::Server_ResetCoolDown_Implementation()
{
    SetTimer();
    SetRunning();
}

void UAbility::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    //Avoid any trouble
    Super::EndPlay(EndPlayReason);
    if(GetOwner()->HasAuthority())
    {
        GetTimerManager()->ClearAllTimersForObject(this);
        CoolDownDelegate.Unbind();
    }
}

void UAbility::OnRep_CanUse()
{

}

void UAbility::OnRep_CoolDownState()
{

}

bool UAbility::Server_IncrementLevel_Validate()
{
    return true; // Security /Sanity check should be done in the Player State Class.
}

void UAbility::Server_IncrementLevel_Implementation()
{
    Level++;
}


void UAbility::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UAbility, CDState);
    DOREPLIFETIME(UAbility, Level);
    //DOREPLIFETIME_CONDITION(URangedAbility, HitResults, COND_SkipOwner);

}
