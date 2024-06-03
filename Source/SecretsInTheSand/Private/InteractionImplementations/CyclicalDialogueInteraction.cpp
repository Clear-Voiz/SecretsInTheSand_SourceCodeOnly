#include "InteractionImplementations/CyclicalDialogueInteraction.h"

#include "Managers/MyPlayerController.h"
#include "Interfaces/Interactable.h"
#include "Kismet/KismetMathLibrary.h"

void UCyclicalDialogueInteraction::ImplementInteraction(ALuredToAdventureCharacter* Subject, AActor* Owner)
{
    AMyPlayerController* MyPlayerController = Subject->GetMyPlayerController();
	if (!MyPlayerController) return;
	Subject->SwitchMappingContext(MyPlayerController->DefaultMappingContext, MyPlayerController->DialoguesMappingContext);
	Subject->SetCanInteract(false);
	if (Owner->Implements<UInteractable>())
	{
		IInteractable* Interactable = CastChecked<IInteractable>(Owner);
		MyPlayerController->AddDialogueHUD(Interactable->GetDialogue(), Interactable->GetFontInfo(), Interactable->GetGlyph());
		
	}
	Subject->ServerRPC_PlayMontage(Subject->CheckingMontage);
	LookAtObject(Subject);
	
}
void UCyclicalDialogueInteraction::ImplementEndInteraction(ALuredToAdventureCharacter *Subject, AActor* Owner)
{
	Subject->ServerRPC_PlayMontage(Subject->CheckingMontage, FName("ReachOutEnd"));
	AMyPlayerController* MyPlayerController = Subject->GetMyPlayerController();
	if (!MyPlayerController) return;
	Subject->SwitchMappingContext(MyPlayerController->DialoguesMappingContext, MyPlayerController->DefaultMappingContext);
	MyPlayerController->RemoveDialogueHUD();
	Subject->SetCanInteract(true);
}