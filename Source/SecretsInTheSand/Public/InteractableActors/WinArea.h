#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Triggerable.h"
#include "WinArea.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinAreaOnWinCondition);
UCLASS()
class SECRETSINTHESAND_API AWinArea : public AActor, public ITriggerable
{
	GENERATED_BODY()
	
public:	
	AWinArea();

	UFUNCTION()
	void Win();

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerArea;


	UPROPERTY(EditAnywhere)
	TObjectPtr<class ULevelSequence> SceneToPlay;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class ALevelSequenceActor> LevelSequenceActor;

	UPROPERTY(BlueprintAssignable, Meta = (AllowPrivateAccess = "true"))
	FWinAreaOnWinCondition OnWinCondition;

	UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> VictoryWidgetClass;

	UPROPERTY()
	bool bWinState = false;

	
	UFUNCTION(BlueprintCallable, Meta = (AllowPrivateAccess = "true"))
    void AddVictoryScreen();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
