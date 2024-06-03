#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Triggerable.h"
#include "MechanismActivator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECRETSINTHESAND_API UMechanismActivator : public UActorComponent, public ITriggerable 
{
	GENERATED_BODY()

public:	
	UMechanismActivator();

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> Mechanisms;

	UFUNCTION()
	void AddTotalActiveMechanisms();

	UFUNCTION()
	void Reset();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	int32 TotalActiveMechanisms = 0;
	
	UPROPERTY(VisibleAnywhere)
	bool bIstriggered = false;

		
};
