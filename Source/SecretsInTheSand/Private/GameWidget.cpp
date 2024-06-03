#include "GameWidget.h"
#include "Components/TextBlock.h"
#include "Managers/MyPlayerController.h"

void UGameWidget::UpdateScore(const int32& DeltaScore)
{
    FString InputString = FString::FromInt(DeltaScore);
    Text_Score->SetText(FText::FromString(InputString));

}

void UGameWidget::UpdateKeys(int DeltaKeys)
{
    TotalKeys += DeltaKeys;
    PlayAnimation(Obtained);
    if (TotalKeys == 1)
    {
       if (AMyPlayerController* PC = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
        PC->AddMilestoneWidget();
    }
}
