#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/LuredToAdventureCharacter.h"
#include "Fonts/SlateFontInfo.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class SECRETSINTHESAND_API IInteractable
{
	GENERATED_BODY()

public:
	virtual const TArray<FText>& GetDialogue() const = 0;

	virtual void Interact(ALuredToAdventureCharacter* Subject) = 0;
	virtual void EndInteraction(ALuredToAdventureCharacter* Subject) = 0;
	virtual struct FSlateFontInfo& GetFontInfo() = 0;
	virtual FText GetGlyph() const = 0;

};
