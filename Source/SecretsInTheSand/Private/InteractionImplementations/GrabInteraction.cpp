#include "InteractionImplementations/GrabInteraction.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"

void UGrabInteraction::ImplementInteraction(ALuredToAdventureCharacter *Subject, AActor* Owner)
{
    if (!Owner) return;
    LastLocation = Owner->GetActorLocation();
    Subject->SwitchMappingContext(Subject->GetDefaultMappingContext(), Subject->GetCarryingMappingContext());
    Subject->ServerRPC_EnterCarryState(Owner);
    LinkToOnCharacterDeath(Subject);
    
}

void UGrabInteraction::ImplementEndInteraction(ALuredToAdventureCharacter *Subject, AActor* Owner)
{
    Subject->SwitchMappingContext(Subject->GetCarryingMappingContext(), Subject->GetDefaultMappingContext());
    Subject->ServerRPC_ExitCarryState();
}
void UGrabInteraction::ApplyPhysicsToInteractor(AActor *Interactor)
{
    Cast<UPrimitiveComponent>(Interactor->GetRootComponent())->SetSimulatePhysics(true);
}
void UGrabInteraction::Reset(FVector IrrelevantVector)
{
    AActor* Owner = GetOwner();
    if (!Owner) return;
    if (UPrimitiveComponent* OwnerPrimitive = Cast<UPrimitiveComponent>(Owner->GetRootComponent()))
    {
        OwnerPrimitive->SetSimulatePhysics(true);
        OwnerPrimitive->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Block);
	    OwnerPrimitive->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Block);
    }
    UBoxComponent* DetectionShape =  Owner->FindComponentByClass<UBoxComponent>();
    if (!DetectionShape) return;
	if (DetectionShape) DetectionShape->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
    FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld,true);
    Owner->DetachFromActor(DetachmentRules);
    Owner->SetActorLocation(LastLocation); 
    Owner->SetActorRotation(FQuat::Identity);   
}

void UGrabInteraction::LookAtObject(ALuredToAdventureCharacter *Subject)
{
    Super::LookAtObject(Subject);
}

void UGrabInteraction::LinkToOnCharacterDeath(ALuredToAdventureCharacter* Subject)
{
    if (Subject->OnCharacterDeath.IsBoundToObject(this)) return;
    Subject->OnCharacterDeath.AddUFunction(this,"Reset");
}
