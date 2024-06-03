#include "Widgets/StartingGuideWidget.h"
#include "Managers/MyPlayerController.h"

void UStartingGuideWidget::NativeConstruct()
{
    PlayAnimation(Entrance);
    if (AMyPlayerController* PC = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
    {
        PC->DisableInput(PC);
        FInputModeUIOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}

void UStartingGuideWidget::CloseWidget()
{
    if (AMyPlayerController* PC = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
    {
        if (APawn* Player = PC->GetPawn()) {Player->EnableInput(PC);}
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }
    RemoveFromParent();
}

FReply UStartingGuideWidget::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
    CloseWidget();
    return FReply::Handled();
}
