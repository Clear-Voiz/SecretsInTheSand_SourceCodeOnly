#include "Widgets/DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Characters/LuredToAdventureCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Fonts/SlateFontInfo.h"

void UDialogueWidget::SetupDialogue(const TArray<FText>& Dialogue, FSlateFontInfo& FontInfo, FText Glyph)
{
    DialogueLines = Dialogue;
    if (FontInfo.HasValidFont()) DialogueText->SetFont(FontInfo);

    if (!Glyph.IsEmpty()) GlyphText->SetText(Glyph);
    UpdateDialogue();
}

void UDialogueWidget::UpdateDialogue()
{
    if (DialogueIndex > DialogueLines.Num()-1)
    {
        DialogueIndex = 0;
        ALuredToAdventureCharacter* MC = Cast<ALuredToAdventureCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
        if (MC) MC->EndInteraction();
        return;
    }
    else
    {
        DialogueText->SetText(DialogueLines[DialogueIndex++]);
    }

}