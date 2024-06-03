#include "Notifies/KillAnimNotify.h"
#include "Traps/CrocoEnemy.h"

void UKillAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    AActor* CrocoActor = MeshComp->GetOwner();
    if (!CrocoActor) return;
    if (CrocoActor->IsA(ACrocoEnemy::StaticClass()))
    {
        CastChecked<ACrocoEnemy>(CrocoActor)->EatPrey();
    }
}