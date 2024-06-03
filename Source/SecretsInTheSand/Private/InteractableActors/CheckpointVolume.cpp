#include "InteractableActors/CheckpointVolume.h"
#include "GameFramework/Character.h"
#include "Interfaces/Death.h"

void ACheckpointVolume::OnMCBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
   if (OtherActor->IsA(ACharacter::StaticClass()))
   {
        CastChecked<IDeath>(OtherActor)->UpdateCheckpoint(GetActorLocation());
   }
}
void ACheckpointVolume::BeginPlay()
{
    Super::BeginPlay();
    OnActorBeginOverlap.AddUniqueDynamic(this, &ACheckpointVolume::OnMCBeginOverlap);
}