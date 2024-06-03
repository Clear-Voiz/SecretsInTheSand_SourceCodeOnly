#include "InteractableActors/WinArea.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/LTA_GameStateBase.h"
#include "Managers/MyPlayerController.h"
#include "Characters/LuredToAdventureCharacter.h"
#include "Blueprint/UserWidget.h"

AWinArea::AWinArea()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;

	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	SetRootComponent(TriggerArea);
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &AWinArea::OnComponentBeginOverlap);
}

void AWinArea::BeginPlay()
{
	Super::BeginPlay();

}

void AWinArea::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (!HasAuthority()) return;
	if (bWinState) return;

	TArray<AActor*> Players;
	GetOverlappingActors(Players, ACharacter::StaticClass());

	if (Players.Num() > 1) 
	{
		bWinState = true;
		Win();
	}
}

void AWinArea::Win()
{
	OnWinCondition.Broadcast();
	OnActivated.Broadcast();
}

void AWinArea::AddVictoryScreen()
{
	if (AMyPlayerController* PC = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
	{
		PC->AddWinWidget();	
	}
}