#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SwitchContextNotify.generated.h"


UCLASS()
class SECRETSINTHESAND_API USwitchContextNotify : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputMappingContext> CurrentMappingContext;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputMappingContext> NextMappingContext;
	
};
