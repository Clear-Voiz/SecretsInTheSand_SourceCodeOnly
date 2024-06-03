#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Lethal.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FKill, AActor*);
UINTERFACE(MinimalAPI)
class ULethal : public UInterface
{
	GENERATED_BODY()
};


class SECRETSINTHESAND_API ILethal
{
	GENERATED_BODY()

	
public:
	FKill OnLethalKill;

};
