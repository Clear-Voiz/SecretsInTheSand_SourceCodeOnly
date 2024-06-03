#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Transporter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECRETSINTHESAND_API UTransporter : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTransporter();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Transport(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void SetLocations(FVector Location1, FVector Location2);
	
	UPROPERTY(EditAnywhere)
	float MoveTime;

	UPROPERTY()
	AActor* TransporterOwner;
	
	UPROPERTY(EditAnywhere)
	TArray<AActor *> TriggerActors;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> Deactivators;

	UPROPERTY(EditAnywhere)
	bool bAllDeactivatorsDeactivation = false;;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOwnerIsTriggerActor = false;

	UFUNCTION()
	void Reset();
	
	UFUNCTION() FORCEINLINE
	int32 GetDeactivatorsTriggerCount() const {return DeactivatorsTriggerCount;}

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	FVector StartLocation;
	UPROPERTY(EditAnywhere)
	FVector EndLocation;
	UPROPERTY(VisibleAnywhere)
	bool bAreLocationsSet;

	UPROPERTY(EditAnywhere, Category="Special Effects")
	TObjectPtr<class USoundBase> MoveToSFX;

	UPROPERTY(EditAnywhere, Category="Special Effects")
	TObjectPtr<class USoundBase> MoveBackSFX;

	UPROPERTY(VisibleAnywhere)
	int32 ActivatedTriggerCount;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bAllTriggerActorsTriggered;

	UPROPERTY(VisibleAnywhere)
	bool bAllDeactivatorsTriggered;

	UPROPERTY(VisibleAnywhere)
	int32 DeactivatorsTriggerCount;


	UFUNCTION()
	void OnMechanismActivated();

	UFUNCTION()
	void OnMechanismDeactivated();
		
};
