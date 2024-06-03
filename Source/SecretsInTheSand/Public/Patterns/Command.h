#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Command.generated.h"

UCLASS(Abstract)
class SECRETSINTHESAND_API UCommand : public UObject
{
	GENERATED_BODY()

public:
	virtual void Execute();
	virtual void Undo();
};
