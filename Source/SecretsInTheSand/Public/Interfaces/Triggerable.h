#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Triggerable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggerActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggerDeactivated);
DECLARE_MULTICAST_DELEGATE(FOnTriggerReset);

UINTERFACE(MinimalAPI)
class UTriggerable : public UInterface
{
	GENERATED_BODY()
};

class SECRETSINTHESAND_API ITriggerable
{
	GENERATED_BODY()

public:

	FOnTriggerActivated OnActivated;

	FOnTriggerDeactivated OnDeactivated;

	FOnTriggerReset OnReset;
};
