#include "Widgets/MilestoneWidget.h"
#include "Managers/MyPlayerController.h"

void UMilestoneWidget::NativeConstruct()
{
    PlayAnimation(Appearance);
}
void UMilestoneWidget::CloseWidget()
{
    if (AMyPlayerController* PC = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
        PC->RemoveMilestoneWidget();
}
FReply UMilestoneWidget::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    CloseWidget();
    return FReply::Handled();
}