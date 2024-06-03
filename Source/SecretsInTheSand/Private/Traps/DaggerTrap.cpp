#include "Traps/DaggerTrap.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/Death.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ADaggerTrap::ADaggerTrap()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Comp"));
	SetRootComponent(RootComp);
	RootComp->SetMobility(EComponentMobility::Movable);

	Appearance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Appearance"));
	Appearance->SetupAttachment(GetRootComponent());
	Appearance->SetCollisionProfileName(FName("OverlapAllDynamic"));

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	DetectionBox->SetupAttachment(GetRootComponent());

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->ProjectileGravityScale = 0.f;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(GetRootComponent());

	GetRootComponent()->SetMobility(EComponentMobility::Movable);

}

void ADaggerTrap::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	if (!HasAuthority()) return;
	if (DetectionBox)
	DetectionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADaggerTrap::OnBoxBeginOverlap);
}

void ADaggerTrap::InitialActivation()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this,&ADaggerTrap::ActivateTrap);
	GetWorldTimerManager().SetTimer(InitialActivationHandle, TimerDelegate, InitialActivationDelay, false);
}

void ADaggerTrap::ActivateTrap()
{
	ProjectileMovementComp->InitialSpeed = 2000.f;
	ProjectileMovementComp->Velocity = GetActorForwardVector()* ProjectileMovementComp->InitialSpeed;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this,&ADaggerTrap::PrepareTrap);
	GetWorldTimerManager().SetTimer(ActivationHandle,TimerDelegate, 1.f, false);
	AudioComponent->Play();
}

void ADaggerTrap::PrepareTrap()
{
	ProjectileMovementComp->Velocity = FVector::ZeroVector;
	SetActorLocation(InitialLocation);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this,&ADaggerTrap::ActivateTrap);
	GetWorldTimerManager().SetTimer(PrepareHandle,TimerDelegate, ActivationRate, false);
	
}

void ADaggerTrap::ResetTrap()
{
	ProjectileMovementComp->Velocity = FVector::ZeroVector;
	SetActorLocation(InitialLocation);
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.ClearAllTimersForObject(this);
}

void ADaggerTrap::OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!OtherActor->Implements<UDeath>()) return;
	if (IDeath* Mortal = CastChecked<IDeath>(OtherActor))
		Mortal->OnCharacterDeath.Broadcast(OtherActor->GetActorLocation());
	if (StabSFX) UGameplayStatics::PlaySoundAtLocation(this, StabSFX, GetActorLocation());
}