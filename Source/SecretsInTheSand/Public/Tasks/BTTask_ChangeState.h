#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Patterns/ECrocoStates.h"
#include "BTTask_ChangeState.generated.h"

UCLASS()
class SECRETSINTHESAND_API UBTTask_ChangeState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ChangeState();
	UPROPERTY(EditAnywhere)
	ECrocoStates NextState;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
