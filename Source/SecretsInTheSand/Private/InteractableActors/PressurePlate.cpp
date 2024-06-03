#include "InteractableActors/PressurePlate.h"
#include "ActorComponents/Transporter.h"

APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	TriggerShape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger Shape"));
	TriggerShape->SetupAttachment(RootComponent);
	TriggerShape->SetIsReplicated(true);

	auto TriggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	if (TriggerMeshAsset.Succeeded())
	{
		TriggerShape->SetStaticMesh(TriggerMeshAsset.Object);
		TriggerShape->SetRelativeScale3D(FVector(3.3f,3.3f,0.2f));
		TriggerShape->SetRelativeLocation(FVector(0.0f,0.0f,10.f));
		TriggerShape->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnBeginOvelap);
		TriggerShape->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnEndOverlap);
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Appearance"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetIsReplicated(true);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Stylized_Egypt/Meshes/building/SM_building_part_08.SM_building_part_08"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D(FVector(4.f,4.f,0.5f));
		Mesh->SetRelativeLocation(FVector(0.0f,0.0f,7.2f));
	}

	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
	Transporter->MoveTime = 0.25f;
	Transporter->bOwnerIsTriggerActor = true;

}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	TriggerShape->SetVisibility(false);
	TriggerShape->SetCollisionProfileName(FName("OverlapAll"));

	Transporter->SetLocations(GetActorLocation(),GetActorLocation()+FVector(0.f,0.f,-10.f));
	
}

void APressurePlate::OnBeginOvelap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!HasAuthority()) return;
	if (bActivated) return;
	if (!OtherActor->ActorHasTag(TEXT("TriggerActor"))) return;
	bActivated = true;
	OnActivated.Broadcast();
}

void APressurePlate::OnEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;
	if (!bActivated) return;
	TArray<AActor*> OverlappingActors;
	TriggerShape->GetOverlappingActors(OverlappingActors);
	for (int i = 0; i<OverlappingActors.Num();++i)
	{
		AActor* Actor = OverlappingActors[i];
		if (Actor->ActorHasTag(TEXT("TriggerActor")))
		return;
	}

	if (bOneTimeAction) return;
	bActivated = false;
	OnDeactivated.Broadcast();
}

