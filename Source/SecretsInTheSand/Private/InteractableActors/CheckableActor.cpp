#include "InteractableActors/CheckableActor.h"
#include "Characters/LuredToAdventureCharacter.h"
#include "Managers/MyPlayerController.h"
#include "Components/BoxComponent.h"
#include "InteractionImplementations/BasicInteraction.h"
#include "Net/UnrealNetwork.h"

ACheckableActor::ACheckableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);
	Appearance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Appearance"));
	Appearance->SetupAttachment(GetRootComponent());
	Appearance->SetGenerateOverlapEvents(true);
	DetectionShape = CreateDefaultSubobject<UBoxComponent>(TEXT("Detection Shape"));
	DetectionShape->SetupAttachment(GetRootComponent());
}

void ACheckableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACheckableActor, Glyph);
}

void ACheckableActor::BeginPlay()
{
	Super::BeginPlay();

	DetectionShape->SetGenerateOverlapEvents(true);
	DetectionShape->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACheckableActor::OnBeginOverlap);
	DetectionShape->OnComponentEndOverlap.AddUniqueDynamic(this, &ACheckableActor::OnEndOverlap);
	check(InteractionType);
	InteractionComponent = NewObject<UBasicInteraction>(this, InteractionType, FName(TEXT("Interaction")),RF_Transient);
	InteractionComponent->RegisterComponent();
}

void ACheckableActor::Interact(ALuredToAdventureCharacter* Subject)
{
	Subject->Interactor = this;
	if (InteractionComponent)  InteractionComponent->ImplementInteraction(Subject,this);
}

void ACheckableActor::EndInteraction(ALuredToAdventureCharacter *Subject)
{
	if (InteractionComponent) InteractionComponent->ImplementEndInteraction(Subject, Subject->Interactor);
	
}
void ACheckableActor::OnBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (ALuredToAdventureCharacter* Player = Cast<ALuredToAdventureCharacter>(OtherActor))
	{
		Player->SetDetection(true,Player->GetVisionDetectionMesh());
		Player->SetInvestigationWidgetVisibility(false);
	}
}
void ACheckableActor::OnEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (ALuredToAdventureCharacter* Player = Cast<ALuredToAdventureCharacter>(OtherActor))
	{
		Player->SetDetection(false, Player->GetVisionDetectionMesh());
		Player->SetInvestigationWidgetVisibility(true);
	}
}

const TArray<FText>& ACheckableActor::GetDialogue() const
{
	return Dialogue;
}