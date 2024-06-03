#pragma once

#include "CoreMinimal.h"
#include "InteractionImplementations/BasicInteraction.h"
#include "CyclicalDialogueInteraction.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class SECRETSINTHESAND_API UCyclicalDialogueInteraction : public UBasicInteraction
{
	GENERATED_BODY()
	
public:
	void ImplementInteraction(class ALuredToAdventureCharacter* Subject, AActor* Owner = nullptr) override;
	void ImplementEndInteraction(class ALuredToAdventureCharacter* Subject, AActor* Owner = nullptr) override;

};
