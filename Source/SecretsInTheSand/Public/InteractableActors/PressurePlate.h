#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Triggerable.h"
#include "PressurePlate.generated.h"



UCLASS()
class SECRETSINTHESAND_API APressurePlate : public AActor, public ITriggerable
{
	GENERATED_BODY()
	
public:	
	APressurePlate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActivated = false;

	UPROPERTY(EditAnywhere)
	bool bOneTimeAction = false;

	
protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TriggerShape;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class UTransporter* Transporter;	

	UFUNCTION()
	void OnBeginOvelap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
