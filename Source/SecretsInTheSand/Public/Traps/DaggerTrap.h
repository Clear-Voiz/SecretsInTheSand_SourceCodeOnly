#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DaggerTrap.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDaggerTrapSpawned, ADaggerTrap*, Trap);

UCLASS()
class SECRETSINTHESAND_API ADaggerTrap : public AActor
{
	GENERATED_BODY()

public:	
	ADaggerTrap();

	UPROPERTY(EditAnywhere)
	float InitialActivationDelay = 0.f;

	UFUNCTION()
	void InitialActivation();

	UFUNCTION()
	void ResetTrap();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Appearance;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> DetectionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere)
	bool bTriggered = false;

	UPROPERTY()
	FTimerHandle ActivationHandle;

	UPROPERTY()
	FTimerHandle PrepareHandle;

	UPROPERTY()
	FTimerHandle InitialActivationHandle;

	UPROPERTY(VisibleAnywhere)
	FVector InitialLocation;

	UPROPERTY(EditAnywhere)
	float ActivationRate = 0.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundBase> StabSFX;

	

	UFUNCTION()
	void ActivateTrap();

	UFUNCTION()
	void PrepareTrap();


	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


};
