#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterBodyRiverTrap.generated.h"

UCLASS()
class SECRETSINTHESAND_API AWaterBodyRiverTrap : public AActor
{
	GENERATED_BODY()

public:
	AWaterBodyRiverTrap();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> NewRoot;

	UFUNCTION()
	void Reset();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPostProcessComponent> PostProcess;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> PostProcessExtents;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTransporter> Transporter;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Appearance;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> InvertedAppearance;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFinalCountdown> FinalCountdown;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UArrowComponent> MaximumWaterLevel;


	
};
