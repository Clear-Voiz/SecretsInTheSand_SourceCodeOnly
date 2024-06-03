#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MilestoneWidget.generated.h"

UCLASS()
class SECRETSINTHESAND_API UMilestoneWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Appearance;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CloseWidget();

	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;


};
