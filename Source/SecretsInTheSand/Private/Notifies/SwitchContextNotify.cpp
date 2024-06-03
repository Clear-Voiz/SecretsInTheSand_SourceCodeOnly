#include "Notifies/SwitchContextNotify.h"
#include "Characters/LuredToAdventureCharacter.h"

void USwitchContextNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    if (!CurrentMappingContext || !NextMappingContext) return;
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;
    if (ALuredToAdventureCharacter* MC = Cast<ALuredToAdventureCharacter>(Owner))
    MC->SwitchMappingContext(CurrentMappingContext,NextMappingContext);

}