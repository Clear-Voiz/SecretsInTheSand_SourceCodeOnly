#include "InteractionImplementations/BasicInteraction.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/LuredToAdventureCharacter.h"

UBasicInteraction::UBasicInteraction()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UBasicInteraction::BeginPlay()
{
	Super::BeginPlay();

}

void UBasicInteraction::LookAtObject(ALuredToAdventureCharacter *Subject)
{
	FRotator LookAtCheckable = UKismetMathLibrary::
	FindLookAtRotation(Subject->GetActorLocation(),GetOwner()->GetActorLocation());
	Subject->SlerpToCheckableObject(Subject->GetActorQuat(), FQuat::MakeFromRotator(LookAtCheckable));
		
}

void UBasicInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

