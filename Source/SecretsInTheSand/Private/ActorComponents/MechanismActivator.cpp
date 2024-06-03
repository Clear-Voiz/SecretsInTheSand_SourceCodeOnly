

#include "ActorComponents/MechanismActivator.h"

UMechanismActivator::UMechanismActivator()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UMechanismActivator::BeginPlay() 
{
	Super::BeginPlay();

	for(TObjectPtr<AActor> Mechanism : Mechanisms)
	{
		ITriggerable* Trigger = Cast<ITriggerable>(Mechanism);
		if (Trigger)
		{
			Trigger->OnActivated.AddUniqueDynamic(this, &UMechanismActivator::AddTotalActiveMechanisms);
		}
	}
	
}

void UMechanismActivator::AddTotalActiveMechanisms()
{
	if (++TotalActiveMechanisms < Mechanisms.Num()) return;
	if (!GetOwner()->Implements<UTriggerable>()) return;
	
	CastChecked<ITriggerable>(GetOwner())->OnDeactivated.Broadcast();

}
void UMechanismActivator::Reset()
{
	TotalActiveMechanisms = 0;
	bIstriggered = false;
}
