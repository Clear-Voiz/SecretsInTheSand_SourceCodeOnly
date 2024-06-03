

#include "ActorComponents/SpinComponent.h"

USpinComponent::USpinComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USpinComponent::BeginPlay()
{
	Super::BeginPlay();

}


void USpinComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetOwner()->AddActorLocalRotation(FRotator(0.f,RotationSpeed * DeltaTime, 0.f));
}

