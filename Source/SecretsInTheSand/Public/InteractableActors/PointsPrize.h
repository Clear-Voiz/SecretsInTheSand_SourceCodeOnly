#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointsPrize.generated.h"

UCLASS()
class SECRETSINTHESAND_API APointsPrize : public AActor
{
	GENERATED_BODY()
	
public:	
	APointsPrize();

	UPROPERTY(EditAnywhere)
	int32 ScoreValue = 5;

	UPROPERTY(EditAnywhere)
	FRotator RotatingMovement = FRotator(0.f,20.f,0.f);

	UFUNCTION(BlueprintImplementableEvent)
	void RevealItself();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Meta=(AllowPrivateAccess="true"))
	class URotatingMovementComponent* RotationComponent;

	UPROPERTY(VisibleAnywhere, Meta=(AllowPrivateAccess="true"))
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Appearance;

	UPROPERTY(VisibleAnywhere, Meta=(AllowPrivateAccess="true"))
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, Category = "Special Effects")
	TObjectPtr<class UNiagaraSystem> PickedVFX;

	UPROPERTY(EditAnywhere, Category = "Special Effects")
	TObjectPtr<class USoundBase> PickedSFX;

	UFUNCTION()
	void OnComponentBeginOverlapp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
