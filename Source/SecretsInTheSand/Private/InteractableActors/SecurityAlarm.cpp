#include "InteractableActors/SecurityAlarm.h"
#include "Managers/MyPlayerController.h"
#include "Engine/TriggerBox.h"
#include "Engine/AssetManager.h"
#include "Components/BoxComponent.h"
#include "OtherActors/KeyCopies.h"
#include "Traps/DaggerTrap.h"
#include "GameFramework/GameModeBase.h"
#include "Traps/WaterBodyRiverTrap.h"
#include "Interfaces/GameModeAdministator.h"
#include "ActorComponents/Transporter.h"
#include "InteractionImplementations/BasicInteraction.h"
#include "InteractableActors/PressurePlate.h"
#include "ActorComponents/MechanismActivator.h"


ASecurityAlarm::ASecurityAlarm()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	ActorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Actor Root"));
	SetRootComponent(ActorRoot);
	Appearance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Appearance"));
	Appearance->SetupAttachment(GetRootComponent());

	ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Child Actor Comp Trigger Alarm"));
	ChildActorComponent->SetupAttachment(GetRootComponent());
	ChildActorComponent->SetChildActorClass(ATriggerBox::StaticClass());

	DetectionShape = CreateDefaultSubobject<UBoxComponent>(TEXT("Detection Shape"));
	DetectionShape->SetupAttachment(GetRootComponent());

}

void ASecurityAlarm::BeginPlay()
{
	Super::BeginPlay();

	if (InteractionType)
	{
		InteractionComponent = NewObject<UBasicInteraction>(this, InteractionType, FName(TEXT("InteractionComponent")), RF_Transient);
		InteractionComponent->RegisterComponent();
	}	

	DetectionShape->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASecurityAlarm::OnBeginOverlap);
	DetectionShape->OnComponentEndOverlap.AddUniqueDynamic(this, &ASecurityAlarm::OnEndOverlap);
	
	if (!HasAuthority()) return;
	ChildActorComponent->GetChildActor()->OnActorBeginOverlap.AddUniqueDynamic(this,&ASecurityAlarm::OnAlarmTriggered);
}

void ASecurityAlarm::OnAlarmTriggered(AActor* OverlappedActor, AActor* OtherActor)
{
	//Authority Checked on BeginPlay()
	if (bActiveMechanism) return;
	if (!OtherActor->ActorHasTag(TEXT("TriggerActor"))) return;
	bActiveMechanism = true;
	OnActivated.Broadcast();
	for(const TObjectPtr<ADaggerTrap> DaggerTrap : DaggerTraps)
	{
		DaggerTrap->InitialActivation();

	}
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (!GameMode) return;
	if (GameMode->Implements<UGameModeAdministator>())
	{
		IGameModeAdministator* GameModeAdmin = CastChecked<IGameModeAdministator>(GameMode);
		GameModeAdmin->AddCommand(this);
	}

}

void ASecurityAlarm::OnBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (bCeilingDeactivated) return;
	if (ALuredToAdventureCharacter* Player = Cast<ALuredToAdventureCharacter>(OtherActor))
	{
		Player->SetDetection(true, Player->GetVisionDetectionMesh());
		Player->SetInvestigationWidgetVisibility(false);
	}
}

void ASecurityAlarm::Reset()
{
	if (AMyPlayerController* PC = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
		PC->DeactivatedAlarms = 0;
	bActiveMechanism = false;
	bCeilingDeactivated = false;
	for(TObjectPtr<ADaggerTrap> DaggerTrap : DaggerTraps)
	{
		DaggerTrap->ResetTrap();
	}

	if (StreamableKeyCopyHandle.IsValid())
	{
		StreamableKeyCopyHandle->CancelHandle();
		StreamableKeyCopyHandle->ReleaseHandle();
	}
	
	if (PressureTrap) PressureTrap->Reset();

	if (TransporterTraps.Num()<=0) return;
	for(TObjectPtr<AActor> TransporterActor : TransporterTraps)
	{
		if (UTransporter* Transporter = TransporterActor->FindComponentByClass<UTransporter>())
			Transporter->Reset();
		if (APressurePlate* PressurePlate = Cast<APressurePlate>(TransporterActor))
		{
			PressurePlate->bActivated = false;
		}
		if (UMechanismActivator* WaterfallSystem = TransporterActor->FindComponentByClass<UMechanismActivator>())
		{
			WaterfallSystem->Reset();
		}
	}

	if (!HasAuthority() || KeyCopy == nullptr) return;
	KeyCopy.Get()->Destroy();
}
void ASecurityAlarm::SpawnKey(TSoftClassPtr<AKeyCopies> Copy)
{
	if (!Copy.IsValid()) return;
	KeyCopy = SpawnObject<AKeyCopies>(Copy.Get());
}

void ASecurityAlarm::OnEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (bCeilingDeactivated) return;
	if (ALuredToAdventureCharacter* Player = Cast<ALuredToAdventureCharacter>(OtherActor))
	{
		Player->SetDetection(false, Player->GetVisionDetectionMesh());
		Player->SetInvestigationWidgetVisibility(true);
	}
}

void ASecurityAlarm::DeactivateSecurityAlarm()
{
	bCeilingDeactivated = true;
	OnDeactivated.Broadcast();
	StreamableKeyCopyHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(KeyCopyClass.ToSoftObjectPath());//, FStreamableDelegate::CreateUObject(this,&ASecurityAlarm::SpawnKey,KeyCopy)
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this,&ASecurityAlarm::SpawnKey,KeyCopyClass);
	GetWorldTimerManager().SetTimer(SpawnObjectHandle,TimerDelegate, 1.f, false);
	for(const TObjectPtr<ADaggerTrap> DaggerTrap : DaggerTraps)
	{ DaggerTrap->ResetTrap(); }
}

void ASecurityAlarm::Interact(ALuredToAdventureCharacter *Subject)
{
	Subject->Interactor = this;
	if (InteractionComponent) InteractionComponent->ImplementInteraction(Subject, this);
	if (AMyPlayerController* PC = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
		PC->ServerRPC_DeactivateSecurityAlarm(this);
}

void ASecurityAlarm::EndInteraction(ALuredToAdventureCharacter *Subject)
{
	Subject->ServerRPC_PlayMontage(Subject->CheckingMontage, FName("ReachOutEnd"));
	Subject->SetDetection(false, Subject->GetVisionDetectionMesh());
	Subject->SetInvestigationWidgetVisibility(true);
	AMyPlayerController* MyPlayerController = Subject->GetMyPlayerController();
	if (!MyPlayerController) return;
	Subject->SwitchMappingContext(MyPlayerController->DialoguesMappingContext, MyPlayerController->DefaultMappingContext);
	MyPlayerController->RemoveDialogueHUD();
}

template <typename T>
inline AActor* ASecurityAlarm::SpawnObject(TSubclassOf<T>  Object)
{
	const FVector SpawnLocation = GetActorLocation() + (FVector::UpVector * 200.f);
	const FRotator SpawnRotation = GetActorRotation();
	return GetWorld()->SpawnActor<T>(Object, SpawnLocation, SpawnRotation);
}

void ASecurityAlarm::BeginDestroy()
{
	Super::BeginDestroy();

	if (!StreamableKeyCopyHandle.IsValid()) return;
	if (StreamableKeyCopyHandle->IsActive())
	{
		StreamableKeyCopyHandle->CancelHandle();
	}
	if (!StreamableKeyCopyHandle.IsValid())
	{
		return;
	}
}

const TArray<FText> &ASecurityAlarm::GetDialogue() const
{
    return Dialogue;
}