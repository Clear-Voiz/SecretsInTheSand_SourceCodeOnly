#include "Tasks/BTTask_Chase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Managers/CrocoAIController.h"


UBTTask_Chase::UBTTask_Chase()
{
    NodeName = TEXT("Chase");
}
EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    return EBTNodeResult::Succeeded;
}