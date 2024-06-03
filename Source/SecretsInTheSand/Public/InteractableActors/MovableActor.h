#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableActor.generated.h"

UCLASS()
class SECRETSINTHESAND_API AMovableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovableActor();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Location1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Location2;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Appearance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	class UTransporter* Transporter;


};
