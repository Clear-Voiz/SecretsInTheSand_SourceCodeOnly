#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ErrorMessageWidget.generated.h"

UCLASS()
class SECRETSINTHESAND_API UErrorMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UTextBlock> ErrorMessage;
};
