#pragma once

#include "CoreMinimal.h"
#include "Patterns/Command.h"
#include "Command_EnableTransport.generated.h"

UCLASS()
class SECRETSINTHESAND_API UCommand_EnableTransport : public UCommand
{
	GENERATED_BODY()

public:
	void Init(class UTransporter& transporter);

private:
	TObjectPtr<class UTransporter> Transporter;
	
};
