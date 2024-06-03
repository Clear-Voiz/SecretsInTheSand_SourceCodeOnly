#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Death.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDeath, FVector);
UINTERFACE(MinimalAPI)
class UDeath : public UInterface
{
	GENERATED_BODY()
};

class SECRETSINTHESAND_API IDeath
{
	GENERATED_BODY()

public:
	FDeath OnCharacterDeath;
	virtual void UpdateCheckpoint(FVector NewLoc) = 0;

	FVector RespawnLocation;
};
