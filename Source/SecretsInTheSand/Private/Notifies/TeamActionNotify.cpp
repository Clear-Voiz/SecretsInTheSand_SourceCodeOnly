#include "Notifies/TeamActionNotify.h"
#include "Components/BoxComponent.h"
#include "Characters/LuredToAdventureCharacter.h"

void UTeamActionNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;
    ALuredToAdventureCharacter* MC = Cast<ALuredToAdventureCharacter>(Owner);
    if (!MC) return;
    MC->SetDetection(bEnableDetection,MC->GetTeamActionDetection());
}
