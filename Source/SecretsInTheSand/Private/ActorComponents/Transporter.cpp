#include "ActorComponents/Transporter.h"
#include "Characters/LuredToAdventureCharacter.h"
#include "InteractableActors/PressurePlate.h"
#include "InteractableActors/CollectableKey.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/LTA_GameStateBase.h"

UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	MoveTime = 3.f;
	ActivatedTriggerCount = 0;
	DeactivatorsTriggerCount = 0;
	bAllDeactivatorsTriggered = true;
	bAreLocationsSet = false;
	StartLocation = FVector().Zero();
	EndLocation = FVector().Zero();

}

void UTransporter::BeginPlay() 
{
	Super::BeginPlay();

	if (!GetOwner()->HasAuthority()) return;
	if (bOwnerIsTriggerActor)
	TriggerActors.Add(GetOwner());

	TransporterOwner = GetOwner();
	bool bIsOwnDeactivator = Deactivators.Num()<1;
	
	for (AActor* TriggerActor : TriggerActors)
	{
		ITriggerable* Triggerable = Cast<ITriggerable>(TriggerActor);
		if (!Triggerable) continue;
		Triggerable->OnActivated.AddDynamic(this, &UTransporter::OnMechanismActivated);
		if (!bIsOwnDeactivator) continue;
		if (!GetOwner()->Implements<UTriggerable>()) continue; 
		CastChecked<ITriggerable>(GetOwner())->OnDeactivated.AddDynamic(this, &UTransporter::OnMechanismDeactivated);
	}

	if (bIsOwnDeactivator) return;
	for (TObjectPtr<AActor> Deactivator : Deactivators)
	{
		ITriggerable* Trigger = Cast<ITriggerable>(Deactivator);
		if (!Trigger) continue;
		Trigger->OnDeactivated.AddUniqueDynamic(this, &UTransporter::OnMechanismDeactivated);
	}
}

void UTransporter::SetLocations(FVector Location1, FVector Location2)
{
	if (Location1 == Location2) return;
	StartLocation = Location1;
	EndLocation = Location2;

	bAreLocationsSet = true;
}

void UTransporter::OnMechanismActivated()
{
	ActivatedTriggerCount += 1;

	if (TriggerActors.Num() == 0) return;
	bAllTriggerActorsTriggered = ActivatedTriggerCount >= TriggerActors.Num();

	if (bAllTriggerActorsTriggered)
	{
		ALTA_GameStateBase* GSB = GetWorld()->GetGameState<ALTA_GameStateBase>();	
		if (MoveToSFX && GSB) GSB->MulticastRPC_PlaySoundAtLocation(MoveToSFX,GetOwner()->GetActorLocation());
		DeactivatorsTriggerCount = Deactivators.Num();
		bAllDeactivatorsTriggered = false;
	}
}

void UTransporter::OnMechanismDeactivated()
{
	if (bAllTriggerActorsTriggered)
	{
		ALTA_GameStateBase* GSB = GetWorld()->GetGameState<ALTA_GameStateBase>();
		if (MoveToSFX && GSB) GSB->MulticastRPC_PlaySoundAtLocation(MoveBackSFX,GetOwner()->GetActorLocation());
	}
	
	--ActivatedTriggerCount;
	bAllTriggerActorsTriggered = ActivatedTriggerCount >= TriggerActors.Num();

	--DeactivatorsTriggerCount;
	bAllDeactivatorsTriggered = DeactivatorsTriggerCount < 1;
}

void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    Transport(DeltaTime);
}

void UTransporter::Transport(float DeltaTime)
{
    if (!TransporterOwner || !TransporterOwner->HasAuthority())
        return;
    if (!bAreLocationsSet)
        return;

    FVector CurrentLocation = TransporterOwner->GetActorLocation();
	FVector TargetLocation;
	if (bAllDeactivatorsDeactivation)
		TargetLocation = bAllDeactivatorsTriggered ? StartLocation : EndLocation;
	else
    	TargetLocation = bAllTriggerActorsTriggered ? EndLocation : StartLocation;
    float Speed = FVector::Distance(StartLocation, EndLocation) / MoveTime;

    if (CurrentLocation != TargetLocation)
    {
        FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
        TransporterOwner->SetActorLocation(NewLocation);
    }
}

void UTransporter::Reset()
{
	ActivatedTriggerCount = 0;
	bAllTriggerActorsTriggered = ActivatedTriggerCount >= TriggerActors.Num();
	DeactivatorsTriggerCount = 0;
	bAllDeactivatorsTriggered = DeactivatorsTriggerCount < 1;
	AActor* Owner = GetOwner();
	if (!Owner) return;
	Owner->SetActorLocation(StartLocation);
}
