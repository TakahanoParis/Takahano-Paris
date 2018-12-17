// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPlayerState.h"
#include "UnrealNetwork.h"
#include "CustomPlayerController.h"


ACustomPlayerState::ACustomPlayerState(): PlayerLife(100), MaxPlayerLife(300), PlayerStamina(100), PlayerStaminaRegenRate(1.0), MaxPlayerStamina(100)
{
}

void ACustomPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	Owner = C;
}

bool ACustomPlayerState::I_TakeDamage(const float &DamageAmount, AActor* Instigator)
{
	if (!Instigator)
		return false;
	Server_TakeDamage(DamageAmount);
	return true;
}

void ACustomPlayerState::RegenerateStamina()
{
	if (PlayerStamina + PlayerStaminaRegenRate < MaxPlayerStamina)
	{
		PlayerStamina += PlayerStaminaRegenRate;
	}
	else
	{
		PlayerStamina = MaxPlayerStamina;
	}
}

void ACustomPlayerState::Tick(float DeltaSeconds)
{
	RegenerateStamina();
}

bool ACustomPlayerState::Server_TakeDamage_Validate(const float& DamageAmount)
{
	return true;
}

void ACustomPlayerState::Server_TakeDamage_Implementation(const float& DamageAmount)
{
	PlayerLife -= DamageAmount;
	if (PlayerLife <= 0)
	{
		const auto aPC = Cast<ACustomPlayerController>(Owner);
		if (!aPC)
			return;
		aPC->OnCharacterDie();
	}
}


void ACustomPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomPlayerState, PlayerTeam);
	DOREPLIFETIME(ACustomPlayerState, PlayerLife);
	DOREPLIFETIME(ACustomPlayerState, PlayerStamina);
	DOREPLIFETIME(ACustomPlayerState, MaxPlayerStamina);
	DOREPLIFETIME(ACustomPlayerState, MaxPlayerLife);
	DOREPLIFETIME(ACustomPlayerState, PlayerStaminaRegenRate);
}

