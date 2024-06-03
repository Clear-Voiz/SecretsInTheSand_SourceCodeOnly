#include "SubManagers/SceneTriggerer.h"
#include "Managers/LTA_GameStateBase.h"
#include "Interfaces/Triggerable.h"

ASceneTriggerer::ASceneTriggerer()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

}

void ASceneTriggerer::BeginPlay()
{
	Super::BeginPlay();

	check(SceneToPlay);
	check(LevelSequenceActor);
	
	for (TObjectPtr<AActor> TriggerActor : TriggerActors)
	{
		if (TriggerActor->Implements<UTriggerable>())
		{
			ITriggerable* Trigger = CastChecked<ITriggerable>(TriggerActor);
			if (!Trigger) return;
			Trigger->OnActivated.AddDynamic(this,&ASceneTriggerer::IncreaseActiveTriggers);
			Trigger->OnDeactivated.AddDynamic(this, &ASceneTriggerer::DecreaseActiveTriggers);
		}
	}
}

void ASceneTriggerer::IncreaseActiveTriggers()
{
	if (++ActiveTriggers >= TriggerActors.Num() && !bAlreadyTriggered) TriggerScene();
}

void ASceneTriggerer::DecreaseActiveTriggers()
{
	if (ActiveTriggers > 0) --ActiveTriggers;
}

void ASceneTriggerer::TriggerScene()
{
	if (!HasAuthority()) return;
	if (ALTA_GameStateBase* GameState = GetWorld()->GetGameState<ALTA_GameStateBase>())
	{
		GameState->MulticastRPC_PlayCinematic(SceneToPlay, LevelSequenceActor);
		bAlreadyTriggered = true;
	}
}

void ASceneTriggerer::AfterScene(float TimerDuration)
{
	
}
