#pragma once

#include "CoreMinimal.h"
#include "InteractionImplementations/BasicInteraction.h"
#include "GrabInteraction.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class SECRETSINTHESAND_API UGrabInteraction : public UBasicInteraction
{
	GENERATED_BODY()

public:
	void ImplementInteraction(class ALuredToAdventureCharacter* Subject, AActor* Owner = nullptr) override;
	void ImplementEndInteraction(class ALuredToAdventureCharacter* Subject, AActor* Owner = nullptr) override;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UFUNCTION()
	void ApplyPhysicsToInteractor(AActor* Interactor);

	UFUNCTION()
	void Reset(FVector IrrelevantVector);

protected:
	void LookAtObject(ALuredToAdventureCharacter* Subject) override;
	FVector LastLocation;
	void LinkToOnCharacterDeath(ALuredToAdventureCharacter* Subject);
};
