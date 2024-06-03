#include "Tasks/BTTask_ChangeState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Managers/CrocoAIController.h"

UBTTask_ChangeState::UBTTask_ChangeState()
{
    NodeName = "Change State";
}
EBTNodeResult::Type UBTTask_ChangeState::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    ACrocoAIController* CrocoAI = Cast<ACrocoAIController>(OwnerComp.GetAIOwner());
    if (!CrocoAI) return EBTNodeResult::Aborted;
    CrocoAI->SetCurrentState(NextState);
    return EBTNodeResult::Succeeded;
}