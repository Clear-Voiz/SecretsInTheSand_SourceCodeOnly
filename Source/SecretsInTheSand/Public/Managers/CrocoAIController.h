#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Patterns/ECrocoStates.h"
#include "CrocoAIController.generated.h"

UCLASS()
class SECRETSINTHESAND_API ACrocoAIController : public AAIController
{
	GENERATED_BODY()

public:
	void SetCurrentState(ECrocoStates State);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBehaviorTree* BehaviorTree;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
};
