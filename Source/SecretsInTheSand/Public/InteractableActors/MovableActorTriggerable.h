#pragma once

#include "CoreMinimal.h"
#include "InteractableActors/MovableActor.h"
#include "Interfaces/Triggerable.h"
#include "MovableActorTriggerable.generated.h"

UCLASS()
class SECRETSINTHESAND_API AMovableActorTriggerable : public AMovableActor, public ITriggerable
{
	GENERATED_BODY()
	
};
