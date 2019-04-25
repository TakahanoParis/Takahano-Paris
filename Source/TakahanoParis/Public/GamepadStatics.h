#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GamepadStatics.generated.h"

/**
*
*/
UCLASS()
class TAKAHANOPARIS_API UGamepadStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* Check if gamepad is connected at Runtime. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GamepadConnected"), Category = "System Information")
		static bool IsGamePadConnected();


};