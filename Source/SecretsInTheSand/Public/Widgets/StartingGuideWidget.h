#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartingGuideWidget.generated.h"

UCLASS()
class SECRETSINTHESAND_API UStartingGuideWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Entrance;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CloseWidget();

	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;


	
};
