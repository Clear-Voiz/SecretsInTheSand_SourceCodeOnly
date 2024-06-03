#include "Tasks/BTTask_Patrol.h"
#include "Traps/CrocoEnemy.h"
#include "Components/SplineComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Patrol::UBTTask_Patrol()
{
    NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    return EBTNodeResult::InProgress;
}
