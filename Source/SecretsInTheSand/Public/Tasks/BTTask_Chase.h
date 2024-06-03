#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Chase.generated.h"

UCLASS()
class SECRETSINTHESAND_API UBTTask_Chase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Chase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Experiment;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
