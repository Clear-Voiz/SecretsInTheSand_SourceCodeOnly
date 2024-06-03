#include "Managers/GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.h"
#include "Widgets/GameOverWidget.h"
#include "Widgets/DialogueWidget.h"
#include "Widgets/HieroglyphsInputWidget.h"
#include "Widgets/MilestoneWidget.h"
#include "Widgets/CJumpGuideWidget.h"
#include "Widgets/ErrorMessageWidget.h"
#include "Widgets/WinScreen.h"
#include "Widgets/StartingGuideWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void AGameHUD::BeginPlay()
{
    GameUI = Cast<UGameWidget>(CreateWidget(PlayerOwner, GameUIClass));
    if (GameUI) GameUI->AddToViewport();

    if (StartingGuideWidget = Cast<UStartingGuideWidget>(CreateWidget(PlayerOwner, StartingGuideClass)))
        StartingGuideWidget->AddToViewport();

    check(GameOverClass);
    check(GlyphsClass);
    check(MilestoneClass);
    check(CJumpGuideClass);
    check(WinScreenClass);
    check(StartingGuideClass);
}
void AGameHUD::CallUpdateKeys(int DeltaAmmount)
{
    if (GameUI) GameUI->UpdateKeys(DeltaAmmount);
}
void AGameHUD::StartDialogue(const TArray<FText>& Dialogue, FSlateFontInfo& FontInfo, FText Glyph)
{
    DialogueWidget = Cast<UDialogueWidget>(CreateWidget(PlayerOwner, DialogueClass));
    if (DialogueWidget) DialogueWidget->AddToViewport();
    DialogueWidget->SetupDialogue(Dialogue, FontInfo, Glyph);
    
}
void AGameHUD::UpdateDialogue()
{
    if (!DialogueWidget) return;
    DialogueWidget->UpdateDialogue();
}

void AGameHUD::EndDialogue()
{
    if (DialogueWidget)
    DialogueWidget->RemoveFromParent();
}

void AGameHUD::UpdateScore(const int32& DeltaAmmount)
{
    if (GameUI) GameUI->UpdateScore(DeltaAmmount);
}

void AGameHUD::AddGameOverScreen()
{
    GameOverWidget = Cast<UGameOverWidget>(CreateWidget(PlayerOwner, GameOverClass));
    if (GameOverWidget) GameOverWidget->AddToViewport();
}

void AGameHUD::AddGlyphsInputWidget()
{
    GlyphsWidget = Cast<UHieroglyphsInputWidget>(CreateWidget(PlayerOwner, GlyphsClass));
    if (GlyphsWidget) GlyphsWidget->AddToViewport();
}

void AGameHUD::RemoveGlyphsInputWidget()
{
    if (GlyphsWidget) GlyphsWidget->RemoveFromParent();
}

void AGameHUD::AddMilestoneWidget()
{
    MilestoneWidget = Cast<UMilestoneWidget>(CreateWidget(PlayerOwner, MilestoneClass));
    if (MilestoneWidget) MilestoneWidget->AddToViewport(-1);
}

void AGameHUD::RemoveMilestoneWidget()
{
    AddCJumpGuideWidget();
    if (MilestoneWidget) MilestoneWidget->RemoveFromParent();
}

void AGameHUD::AddCJumpGuideWidget()
{
    CJumpGuideWidget = Cast<UCJumpGuideWidget>(CreateWidget(PlayerOwner, CJumpGuideClass));
    if (CJumpGuideWidget) CJumpGuideWidget->AddToViewport();
}

void AGameHUD::AddErrorMessage(FText Message)
{
    FTimerManager& TimerManager = GetWorldTimerManager();

    if (ErrorMessageWidget) 
    {
        ErrorMessageWidget->RemoveFromParent();
        if (TimerManager.IsTimerActive(RemoveErrorHandle))
            TimerManager.ClearTimer(RemoveErrorHandle);
    }
    ErrorMessageWidget = Cast<UErrorMessageWidget>(CreateWidget(PlayerOwner, ErrorMessageClass));
    if (ErrorMessageWidget) 
    {
        ErrorMessageWidget->AddToViewport();
        ErrorMessageWidget->ErrorMessage->SetText(Message);
        FTimerDelegate ErrorDelegate = FTimerDelegate::CreateUFunction(this, "RemoveErrorMessage");
        TimerManager.SetTimer(RemoveErrorHandle,ErrorDelegate, 5.f, false);
    
    }
}

void AGameHUD::RemoveErrorMessage()
{
    if (ErrorMessageWidget)
        ErrorMessageWidget->RemoveFromParent();
}

void AGameHUD::AddWinScreen(int32 FinalResult)
{
    if (WinScreenWidget = Cast<UWinScreen>(CreateWidget(PlayerOwner, WinScreenClass)))
    {
        WinScreenWidget->AddToViewport(-1);
        FString FinalTextNum = FString::FromInt(FinalResult);
        FinalTextNum.Append("/5");
        WinScreenWidget->FinalScore->SetText( FText::FromString(FinalTextNum));
    }
}
