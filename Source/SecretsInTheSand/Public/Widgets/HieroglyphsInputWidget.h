#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HieroglyphsInputWidget.generated.h"


DECLARE_DELEGATE_OneParam(FOnCheckInputGlyphs, const FText&);
class UButton;
UCLASS()
class SECRETSINTHESAND_API UHieroglyphsInputWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Stop_btn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPanelWidget> Grid;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> InputTextBox;

	UPROPERTY()
	FString IntroducedText;

	FOnCheckInputGlyphs OnCheckInputGlyphs;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UButton>> GlyphButtons;


protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void WriteGlyph(int32 ButtonIndex);

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class UNiagaraSystemWidget> NS_Dust;

};
