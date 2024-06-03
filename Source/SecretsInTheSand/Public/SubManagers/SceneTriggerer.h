#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SceneTriggerer.generated.h"

UCLASS()
class SECRETSINTHESAND_API ASceneTriggerer : public AActor
{
	GENERATED_BODY()
	
public:	
	ASceneTriggerer();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(EditAnywhere, Category = Cinematics)
	TObjectPtr<class ULevelSequence> SceneToPlay;

	UPROPERTY(EditAnywhere, Category = Cinematics)
	TObjectPtr<class ALevelSequenceActor> LevelSequenceActor;

	UPROPERTY(EditAnywhere, Category = Cinematics)
	TArray<TObjectPtr<AActor>> TriggerActors;

	UPROPERTY(VisibleAnywhere, Category = Cinematics)
	int32 ActiveTriggers;

	UPROPERTY(VisibleAnywhere, Category = Cinematics)
	bool bAlreadyTriggered = false;

	UPROPERTY()
	FTimerHandle CinematicHandle;


	UFUNCTION()
	void IncreaseActiveTriggers();

	UFUNCTION()
	void DecreaseActiveTriggers();

	UFUNCTION()
	void TriggerScene();

	UFUNCTION()
	void AfterScene(float TimerDuration);
};
