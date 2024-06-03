#include "ActorComponents/FinalCountdown.h"
#include "ActorComponents/Transporter.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/LuredToAdventureCharacter.h"
#include "Managers/LuredToAdventureGameMode.h"
#include "Interfaces/Triggerable.h"

UFinalCountdown::UFinalCountdown()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

}


void UFinalCountdown::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()->HasAuthority()) return;

	AActor* Owner= GetOwner();
	if (Owner)
	{
		Transporter = Cast<UTransporter>(Owner->FindComponentByClass(UTransporter::StaticClass()));
		Duration = Transporter->MoveTime;
		if (Transporter->TriggerActors.Num()>0)
		{
			ITriggerable* TriggerActor = Cast<ITriggerable>(Transporter->TriggerActors[0]);
			TriggerActor->OnActivated.AddUniqueDynamic(this, &UFinalCountdown::StartCountdown);
			}
		if (Transporter->Deactivators.Num()<1) return;
		for(TObjectPtr<AActor> Deactivator : Transporter->Deactivators)
		{
			ITriggerable* Triggerable = Cast<ITriggerable>(Deactivator);
			if (Triggerable) Triggerable->OnDeactivated.AddUniqueDynamic(this, &UFinalCountdown::StopCountdown);
		}
	}


	APlayerController* PC = (GetWorld()->GetFirstPlayerController());
	if (!PC) {return;}
	IDeath* MC = Cast<IDeath>(PC->GetPawn());
	if (MC)
	{
		MC->OnCharacterDeath.AddUFunction(this, "StopCountdown");
	}
}

void UFinalCountdown::StartCountdown()
{
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&UFinalCountdown::Execute,GetOwner());
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,Duration, false);
}

void UFinalCountdown::StopCountdown()
{
	if (!Transporter) return;
	if (Transporter->GetDeactivatorsTriggerCount()>0) return;
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			if (ALuredToAdventureGameMode* GameMode = GetWorld()->GetAuthGameMode<ALuredToAdventureGameMode>())
				GameMode->StopSpawnedSounds();
		}
		
}

void UFinalCountdown::Execute(AActor *Instigator)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		IDeath* MC = Cast<IDeath>(PC->GetPawn());
		if (!MC) return;
		MC->OnCharacterDeath.Broadcast(Instigator->GetActorLocation());
	}
}
