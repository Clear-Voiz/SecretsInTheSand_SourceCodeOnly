#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyCopies.generated.h"

UCLASS()
class SECRETSINTHESAND_API AKeyCopies : public AActor
{
	GENERATED_BODY()

public:	
	AKeyCopies();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class USpinComponent* SpinComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Appearance;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

};
