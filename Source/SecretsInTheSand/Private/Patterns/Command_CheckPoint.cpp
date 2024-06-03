#include "Patterns/Command_CheckPoint.h"


void UCommand_CheckPoint::Init(AActor* character, FVector moveLocation)
{Character=character;MoveLocation=moveLocation;}

void UCommand_CheckPoint::Execute()
{
    Super::Execute();
    
}
