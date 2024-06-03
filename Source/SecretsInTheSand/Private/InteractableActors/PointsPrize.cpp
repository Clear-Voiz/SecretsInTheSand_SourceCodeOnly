#include "InteractableActors/PointsPrize.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Managers/LTA_GameStateBase.h"
#include "Interfaces/GameModeAdministator.h"

APointsPrize::APointsPrize()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(RootComp);
	RotationComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationComponent"));
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	Appearance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Appearance"));

	CollisionComponent->SetupAttachment(GetRootComponent());
	Appearance->SetupAttachment(GetRootComponent());
	
}

void APointsPrize::BeginPlay()
{
	Super::BeginPlay();

	RotationComponent->RotationRate = RotatingMovement;
	CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &APointsPrize::OnComponentBeginOverlapp);
	
}

void APointsPrize::OnComponentBeginOverlapp(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!HasAuthority()) return;

	FVector PointsLocation(GetActorLocation());

	if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
	{
		if (GM->Implements<UGameModeAdministator>())
		{
			IGameModeAdministator* GMAdmin = CastChecked<IGameModeAdministator>(GM);
			if (!GMAdmin->GetCharacters().FindByPredicate([&OtherActor](AActor* Character){return OtherActor == Character;}))
			return;	
			
			GMAdmin->AddCommandAndUpdateScore(GetClass(),ScoreValue,PointsLocation);
		}
	}
	//GM->AddCommandAndUpdateScore(this->GetClass(),ScoreValue, PointsLocation);
	if (ALTA_GameStateBase* GSB = GetWorld()->GetGameState<ALTA_GameStateBase>())
	{
		if (PickedVFX) GSB->MulticastRPC_SpawnParticleAtLocation(PickedVFX, PointsLocation);
		if (PickedSFX) GSB->MulticastRPC_PlaySoundAtLocation(PickedSFX,PointsLocation);
	}
	
	Destroy();
}
