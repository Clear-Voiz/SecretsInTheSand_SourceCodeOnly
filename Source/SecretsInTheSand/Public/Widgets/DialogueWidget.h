#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

UCLASS()
class SECRETSINTHESAND_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FText> DialogueLines;

	UPROPERTY(BlueprintReadOnly)
	int32 DialogueIndex = 0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* DialogueText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> GlyphText;

	UFUNCTION()
	void SetupDialogue(const TArray<FText>& Dialogue, FSlateFontInfo& FontInfo, FText Glyph);

	UFUNCTION(BlueprintCallable)
	void UpdateDialogue();


};
