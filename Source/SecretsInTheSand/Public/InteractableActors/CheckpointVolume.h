#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "CheckpointVolume.generated.h"

UCLASS()
class SECRETSINTHESAND_API ACheckpointVolume : public ATriggerVolume
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void OnMCBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
