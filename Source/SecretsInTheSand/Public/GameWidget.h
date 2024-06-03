#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

UCLASS()
class SECRETSINTHESAND_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Meta=(BindWidget))
	class UTextBlock* Text_Score;

	UPROPERTY(Transient, BlueprintReadOnly, meta=(BindWidgetAnim))
	UWidgetAnimation* Obtained;

	UPROPERTY()
	int32 TotalKeys;

	UFUNCTION()
	void UpdateScore(const int32& DeltaScore);

	UFUNCTION()
	void UpdateKeys(int DeltaKeys);
	
	UFUNCTION() FORCEINLINE
	int32 GetTotalKeys() const {return TotalKeys;}
};
