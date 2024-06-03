#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinScreen.generated.h"

UCLASS()
class SECRETSINTHESAND_API UWinScreen : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWindget))
	TObjectPtr<class UTextBlock> FinalScore;

};
