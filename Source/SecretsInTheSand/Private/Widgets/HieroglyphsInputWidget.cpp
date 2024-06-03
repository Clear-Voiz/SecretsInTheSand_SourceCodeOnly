#include "Widgets/HieroglyphsInputWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SubManagers/GlyphsManager.h"
#include "NiagaraSystemWidget.h"



void UHieroglyphsInputWidget::NativeConstruct()
{
    AActor* PreGlyphsManager = UGameplayStatics::GetActorOfClass(this, AGlyphsManager::StaticClass());
    if (AGlyphsManager* GlyphsManager = CastChecked<AGlyphsManager>(PreGlyphsManager))
    {
        if (GlyphsManager->bAlreadyActivated)
        {
            InputTextBox->SetText(GlyphsManager->GetPassword());
            return;
        }
    }
    TArray<UWidget*> Containedwidgets = Grid->GetAllChildren();
    for (UWidget* ContainedWidget : Containedwidgets)
    {
        if (UButton* Button = Cast<UButton>(ContainedWidget))
            GlyphButtons.Add(Button);
    }

    for (int32 i = 0; i<GlyphButtons.Num(); i++)
    {
        SButton* UnderlyingButton = (SButton*)&GlyphButtons[i]->TakeWidget().Get();
        UnderlyingButton->SetOnClicked(FOnClicked::CreateLambda([this,i]()
        {
            WriteGlyph(i);
            NS_Dust->ActivateSystem(true);
        return FReply::Handled();}));
    }
}


void UHieroglyphsInputWidget::WriteGlyph(int32 ButtonIndex) 
{
    UWidget* PreText = GlyphButtons[ButtonIndex]->GetChildAt(0);
    if (PreText->IsA(UTextBlock::StaticClass()))
    {
        UTextBlock* Text = CastChecked<UTextBlock>(PreText);
        FString RecievedGlyph = Text->GetText().ToString();
        IntroducedText.Append(RecievedGlyph);
        InputTextBox->SetText(FText::FromString(IntroducedText));
        if (IntroducedText.Len() == 6)
        {
            OnCheckInputGlyphs.ExecuteIfBound(InputTextBox->GetText());
        }
        else if (IntroducedText.Len() > 6)
        {
            InputTextBox->SetText(FText::FromString(""));
            IntroducedText.Reset();
        }
        
    }
}