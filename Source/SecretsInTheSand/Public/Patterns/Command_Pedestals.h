#pragma once

#include "CoreMinimal.h"
#include "Patterns/Command.h"
#include "Command_Pedestals.generated.h"

UCLASS()
class SECRETSINTHESAND_API UCommand_Pedestals : public UCommand
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Init(class ASecurityAlarm* securityAlarm);

	void Undo() override;

private:
	UPROPERTY()
	TObjectPtr<class ASecurityAlarm> SecurityAlarm;

};
