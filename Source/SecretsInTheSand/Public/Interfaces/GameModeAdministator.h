#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameModeAdministator.generated.h"

UINTERFACE(MinimalAPI)
class UGameModeAdministator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SECRETSINTHESAND_API IGameModeAdministator
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void AddCommand(class ASecurityAlarm* Alarm) = 0;

	UFUNCTION()
	virtual void AddCommandAndUpdateScore(UClass* ItemType, const int32& Amount, const FVector& Location) = 0;

	UFUNCTION()
	virtual void ClearCommands() = 0;

	virtual TArray<AActor*> GetCharacters() const = 0;

	UFUNCTION()
	virtual void UnlockComboJump() = 0;

};
