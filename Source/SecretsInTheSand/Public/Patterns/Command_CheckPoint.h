#pragma once

#include "CoreMinimal.h"
#include "Patterns/Command.h"
#include "Command_CheckPoint.generated.h"

UCLASS()
class SECRETSINTHESAND_API UCommand_CheckPoint : public UCommand
{
	GENERATED_BODY()

public:
	void Init(AActor* character, FVector moveLocation);
	virtual void Execute() override;

private:
	TObjectPtr<AActor> Character;
	FVector MoveLocation;
	
};
