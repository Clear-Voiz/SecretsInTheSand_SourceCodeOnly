#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/Triggerable.h"
#include "CollectableKey.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCollectableKeyOnCollected);

UCLASS()
class SECRETSINTHESAND_API ACollectableKey : public AActor, public ITriggerable
{
	GENERATED_BODY()
	
public:	
	ACollectableKey();
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, ReplicatedUsing = OnRep_bISCollected)
	bool bIsCollected = false;

	FCollectableKeyOnCollected OnKeyCollected;

	UFUNCTION()
	void OnRep_bISCollected();

protected:
	virtual void BeginPlay() override;	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Appearance;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class URectLightComponent> GlowingLight;

	UPROPERTY(EditAnywhere)
	float Amplitude = 1.f;

	UPROPERTY(EditAnywhere)
	float SinSpeed = 1.f;

	UPROPERTY(VisibleAnywhere)
	float RunningTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 180.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAudioComponent> CollectedSFX;

	UFUNCTION()
	void OnComponentBeginOverlapp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
