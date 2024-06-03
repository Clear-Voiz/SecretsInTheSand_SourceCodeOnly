#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpinComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECRETSINTHESAND_API USpinComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USpinComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 180.f;

		
};
