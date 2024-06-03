#include "Patterns/Command_Pedestals.h"
#include "InteractableActors/SecurityAlarm.h"

void UCommand_Pedestals::Init(ASecurityAlarm *securityAlarm)
{
    SecurityAlarm = securityAlarm;
}

void UCommand_Pedestals::Undo()
{
    Super::Undo();

    SecurityAlarm->Reset();
}
