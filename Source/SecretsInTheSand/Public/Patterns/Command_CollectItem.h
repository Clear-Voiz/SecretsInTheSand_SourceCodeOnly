#pragma once

#include "CoreMinimal.h"
#include "Patterns/Command.h"
#include "Command_CollectItem.generated.h"

UCLASS()
class SECRETSINTHESAND_API UCommand_CollectItem : public UCommand
{
	GENERATED_BODY()
public:
	void Init(UClass* Item, int32 Points, const FVector Location);
	void Undo() override;
	FORCEINLINE 
	int32 GetEarnedPoints() const {return EarnedPoints;}

private:
	TObjectPtr<UClass> PickedItem;
	int32 EarnedPoints;
	FVector SpawnPoint;
};
