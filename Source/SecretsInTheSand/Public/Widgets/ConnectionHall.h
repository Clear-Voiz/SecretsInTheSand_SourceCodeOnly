#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConnectionHall.generated.h"

UCLASS()
class SECRETSINTHESAND_API UConnectionHall : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UTextBlock> ErrorText;
};
