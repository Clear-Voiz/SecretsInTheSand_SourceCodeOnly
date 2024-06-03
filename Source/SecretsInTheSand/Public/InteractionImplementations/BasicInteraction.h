#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicInteraction.generated.h"


UCLASS( ClassGroup=(Custom), Abstract)
class SECRETSINTHESAND_API UBasicInteraction : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBasicInteraction();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	virtual void ImplementInteraction(class ALuredToAdventureCharacter* Subject, AActor* Owner = nullptr) PURE_VIRTUAL(UBasicInteraction::ImplementInteraction,);

	UFUNCTION()
	virtual void ImplementEndInteraction(class ALuredToAdventureCharacter* Subject, AActor* Owner = nullptr) PURE_VIRTUAL(UBasicInteraction::ImplementEndInteraction,);

protected:
	virtual void BeginPlay() override;
	
	FTimerHandle LookAtHandle;

	UFUNCTION()
	virtual void LookAtObject(class ALuredToAdventureCharacter* Subject);

		
};
