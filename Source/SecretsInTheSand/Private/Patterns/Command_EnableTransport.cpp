#include "Patterns/Command_EnableTransport.h"
#include "ActorComponents/Transporter.h"

void UCommand_EnableTransport::Init(UTransporter &transporter)
{
    Transporter = &transporter;
}
