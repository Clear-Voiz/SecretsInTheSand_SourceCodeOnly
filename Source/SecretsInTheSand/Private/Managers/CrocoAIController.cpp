#include "Managers/CrocoAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Navigation/PathFollowingComponent.h"
#include "Traps/CrocoEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"


void ACrocoAIController::BeginPlay()
{
    Super::BeginPlay();
    
}

void ACrocoAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    
    if (!HasAuthority()) return;
    if (BehaviorTree) 
    {
        RunBehaviorTree(BehaviorTree);
        UBlackboardComponent* BB;
        UseBlackboard(BehaviorTree->BlackboardAsset, BB);
        Blackboard = BB;
        ACrocoEnemy* Croco = GetPawn<ACrocoEnemy>();
        Blackboard->SetValueAsObject(TEXT("SelfActor"), Croco);
        Blackboard->SetValueAsVector(TEXT("InitialLocation"), Croco->GetActorLocation());
        Croco->OnPreyDetected.BindLambda([this](ECrocoStates State){SetCurrentState(State);});
    }
}

void ACrocoAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult &Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (!HasAuthority()) return;
    ACrocoEnemy* Croco = GetPawn<ACrocoEnemy>();
    if (!Croco) return;
    if (Result.IsSuccess())
    {
        if (Croco->CurrentState == ECrocoStates::EEE_ReturnToOrigins)
        SetCurrentState(ECrocoStates::EEE_Idle);
    }
}

void ACrocoAIController::SetCurrentState(ECrocoStates State)
{
     if (ACrocoEnemy* CrocoEnemy = GetPawn<ACrocoEnemy>()) CrocoEnemy->CurrentState = State;
    GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), (uint8)State);
}

