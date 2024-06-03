#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvestigationWidget.generated.h"

UCLASS()
class SECRETSINTHESAND_API UInvestigationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchWidget(int index);
};
