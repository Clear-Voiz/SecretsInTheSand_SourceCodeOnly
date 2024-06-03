#include "Widgets/CJumpGuideWidget.h"
#include "MediaPlayer.h"
#include "MediaSource.h"

void UCJumpGuideWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MediaPlayer)
    {
        if (KneelExample) MediaPlayer->OpenSource(KneelExample);
    }

    if (MediaPlayer2)
    {
        if (ImpulseExample) MediaPlayer2->OpenSource(ImpulseExample);
    }
}
