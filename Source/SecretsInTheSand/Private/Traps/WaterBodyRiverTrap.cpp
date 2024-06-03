#include "Traps/WaterBodyRiverTrap.h"
#include "ActorComponents/FinalCountdown.h"
#include "ActorComponents/Transporter.h"
#include "Components/BoxComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/ArrowComponent.h"

AWaterBodyRiverTrap::AWaterBodyRiverTrap()
{
    bReplicates = true;
    SetReplicateMovement(true);

    NewRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
    Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
    FinalCountdown = CreateDefaultSubobject<UFinalCountdown>(TEXT("Final Countdown"));
    MaximumWaterLevel = CreateDefaultSubobject<UArrowComponent>(TEXT("Maximum Water Level"));
    PostProcessExtents = CreateDefaultSubobject<UBoxComponent>(TEXT("PostProcessExtents"));
    PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    Appearance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Appearance"));
    InvertedAppearance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Inverted Appearance"));
    
    SetRootComponent(NewRoot);
    Appearance->SetupAttachment(GetRootComponent());
    Appearance->SetCollisionProfileName(FName("OverlapAllDynamic"));
    InvertedAppearance->SetupAttachment(GetRootComponent());
    InvertedAppearance->SetCollisionProfileName(FName("OverlapAllDynamic"));
    PostProcessExtents->SetupAttachment(GetRootComponent());
    PostProcess->SetupAttachment(PostProcessExtents);
    PostProcess->bUnbound = false;

    MaximumWaterLevel->SetupAttachment(GetRootComponent());


    GetRootComponent()->SetMobility(EComponentMobility::Movable);
}

void AWaterBodyRiverTrap::BeginPlay() 
{
    Super::BeginPlay();

    FVector WaterLevel = MaximumWaterLevel->GetComponentLocation();
    Transporter->SetLocations(GetActorLocation(),WaterLevel);
    FinalCountdown->Duration = Transporter->MoveTime;
}

void AWaterBodyRiverTrap::Reset()
{
    Transporter->Reset();
    FinalCountdown->StopCountdown();
}
