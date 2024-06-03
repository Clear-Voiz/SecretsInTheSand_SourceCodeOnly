#include "OtherActors/KeyCopies.h"
#include "Components/CapsuleComponent.h"
#include "ActorComponents/SpinComponent.h"

AKeyCopies::AKeyCopies()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	Appearance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Appearance"));
	Appearance->SetupAttachment(GetRootComponent());

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));
	CapsuleComponent->SetupAttachment(GetRootComponent());
	
	SpinComponent = CreateDefaultSubobject<USpinComponent>(TEXT("Spin Component"));


}

void AKeyCopies::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKeyCopies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

