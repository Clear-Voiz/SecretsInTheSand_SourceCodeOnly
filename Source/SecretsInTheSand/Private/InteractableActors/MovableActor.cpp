#include "InteractableActors/MovableActor.h"
#include "Components/ArrowComponent.h"
#include "ActorComponents/Transporter.h"


AMovableActor::AMovableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	Location1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Start Location"));
	Location2 = CreateDefaultSubobject<UArrowComponent>(TEXT("End Location"));

	Location1->SetupAttachment(RootComponent);
	Location2->SetupAttachment(RootComponent);

	Location1->SetRelativeLocation(FVector::Zero());
	Location2->SetRelativeLocation(FVector(0.f,0.f,300.f));

	Appearance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Appearance"));
	Appearance->SetupAttachment(RootComponent);
	Appearance->SetIsReplicated(true);

	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));

}

void AMovableActor::BeginPlay()
{
	Super::BeginPlay();
	FVector WorldLoc1 = Location1->GetRelativeLocation() + GetActorLocation();
	FVector WorldLoc2 = Location2->GetRelativeLocation() + GetActorLocation();
	Transporter->SetLocations(WorldLoc1, WorldLoc2);
}
