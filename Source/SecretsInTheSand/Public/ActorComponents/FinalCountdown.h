#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Lethal.h"
#include "Engine/TimerHandle.h"
#include "FinalCountdown.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECRETSINTHESAND_API UFinalCountdown : public UActorComponent, public ILethal
{
	GENERATED_BODY()

public:	
	UFinalCountdown();

	UPROPERTY(EditAnywhere)
	float Duration = 1.f;

	UFUNCTION()
	void StopCountdown();
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<class UTransporter> Transporter;

	UPROPERTY()
	FTimerHandle TimerHandle;

private:	
	UFUNCTION()
	void StartCountdown();

	UFUNCTION()
	void Execute(AActor* Instigator);

};
