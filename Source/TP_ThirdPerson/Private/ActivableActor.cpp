#include "ActivableActor.h"
#include "ThirdPersonCharacter.h"
#include "TP_ThirdPerson.h"

AActivableActor::AActivableActor() : Super() , bIsUsable(true), MinimumUseDistance(2000.f)
{
	
}


bool AActivableActor::CanUse(AThirdPersonCharacter* UserCharacter)
{
	for(const UClass  * it : AuthorizedClass)
	{
		if(UserCharacter->IsA(it) && FVector::Dist(UserCharacter->GetActorLocation(), GetActorLocation()) < MinimumUseDistance)
			return bIsUsable;
	}
	return false;
}

void AActivableActor::Use_Implementation(AThirdPersonCharacter * PlayerCharacter, bool &bShouldLockMovement)
{
#if WITH_EDITOR
	if(PlayerCharacter)
	UE_LOG(LogTP_ThirdPerson, Warning, TEXT("Use  : %s is used by %s"), *GetName(), *PlayerCharacter->GetName());
#endif // WITH_EDITORS
}

FName AActivableActor::GetUsableName() const
{
	return FName(*GetName());
}
