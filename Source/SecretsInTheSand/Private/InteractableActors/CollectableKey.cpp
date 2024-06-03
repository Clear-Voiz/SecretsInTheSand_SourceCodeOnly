#include "InteractableActors/CollectableKey.h"
#include "Components/CapsuleComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"
#include "Characters/LuredToAdventureCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/GameModeAdministator.h"
#include "Managers/MyPlayerController.h"
#include "Managers/LTA_GameStateBase.h"

ACollectableKey::ACollectableKey()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);
	Appearance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Appearance"));
	Appearance->SetupAttachment(RootComponent);
	Appearance->SetIsReplicated(true);
	Appearance->SetCollisionProfileName(FName("OverlapAllDynamic"));

	GlowingLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Glowing Light"));
	GlowingLight->SetupAttachment(Appearance);
	GlowingLight->SetCastShadows(false);
	GlowingLight->SetMobility(EComponentMobility::Movable);
	GlowingLight->SetWorldRotation(FRotator(-90.f,-90.f,0.f));
	GlowingLight->AddRelativeLocation(FVector(0.f,0.f,38.f));
	GlowingLight->SetLightColor(FColor::FromHex("FFF300FF"));
	GlowingLight->SetAttenuationRadius(288.f);
	GlowingLight->SetSourceWidth(358.f);
	GlowingLight->SetSourceHeight(121.f);
	GlowingLight->SetBarnDoorLength(9.f);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
	Capsule->SetIsReplicated(true);
	Capsule->SetCollisionProfileName(FName("OverlapAllDynamic"));
	Capsule->SetCapsuleHalfHeight(150.f);
	Capsule->SetCapsuleRadius(100.f);

	CollectedSFX = CreateDefaultSubobject<UAudioComponent>(TEXT("Collected Audio Component"));
	CollectedSFX->SetupAttachment(RootComponent);
	CollectedSFX->SetAutoActivate(false);

}

void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACollectableKey::OnComponentBeginOverlapp);
}

void ACollectableKey::OnComponentBeginOverlapp(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!HasAuthority()) return;
	ALuredToAdventureCharacter* MC = Cast<ALuredToAdventureCharacter>(OtherActor);
	if (!MC) return;
	if (bIsCollected) return;
	if (!MC->bHasTreasure) 
	{
		bIsCollected = true;
		MC->RespawnLocation = GetActorLocation();
		MC->bHasTreasure = true;
		OnRep_bISCollected();
		if (IGameModeAdministator* GameAdmin = Cast<IGameModeAdministator>(GetWorld()->GetAuthGameMode()))
		{
			GameAdmin->UnlockComboJump();
		}
	}
	else
	{
		if (AMyPlayerController* PC = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
		{
			PC->AddErrorMessage(FText::FromString("<You are already carrying a Treasure (only one per player)>"));
		}
	}


}

void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority()) return;
	RunningTime += DeltaTime;
	if (RunningTime >= PI *2) RunningTime -= PI*2;
	float Offset = Amplitude * FMath::Sin(RunningTime * SinSpeed);

	Appearance->AddLocalOffset(FVector::UpVector * Offset);
}

void ACollectableKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollectableKey, bIsCollected);
}

void ACollectableKey::OnRep_bISCollected()
{
	if (bIsCollected) OnActivated.Broadcast();
	if (IGameModeAdministator* GameAdmin = Cast<IGameModeAdministator>(GetWorld()->GetAuthGameMode()))
	{
		GameAdmin->ClearCommands();
	}

	SetActorHiddenInGame(bIsCollected);

	CollectedSFX->Play();
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController) PlayerController->UpdateKeysHUD(1);
}