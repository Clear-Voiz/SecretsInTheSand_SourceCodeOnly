#include "Tasks/BTTask_Idle.h"


UBTTask_Idle::UBTTask_Idle()
{
    NodeName = TEXT("Idle");
}

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    return EBTNodeResult::InProgress;
}
