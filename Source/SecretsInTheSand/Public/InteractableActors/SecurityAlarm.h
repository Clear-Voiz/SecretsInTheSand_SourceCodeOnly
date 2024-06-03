#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/Interactable.h"
#include "../Interfaces/Triggerable.h"
#include "Engine/StreamableManager.h"
#include "SecurityAlarm.generated.h"

class ADaggerTrap;
UCLASS()
class SECRETSINTHESAND_API ASecurityAlarm : public AActor, public IInteractable, public ITriggerable
{
	GENERATED_BODY()

public:	
	ASecurityAlarm();

	void Interact(ALuredToAdventureCharacter* Subject) override;

	void EndInteraction(ALuredToAdventureCharacter* Subject) override;

	UPROPERTY(EditAnywhere, Category = Interaction)
	TSubclassOf<class UBasicInteraction> InteractionType;

	UPROPERTY(VisibleAnywhere, Category = Interaction)
	TObjectPtr<class UBasicInteraction> InteractionComponent;

	UPROPERTY(EditAnywhere)
	FSlateFontInfo DialogueFont;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* DetectionShape;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AWaterBodyRiverTrap> PressureTrap; //River trap + Descending Ceiling

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> TransporterTraps;

	UPROPERTY(EditAnywhere)
	TArray<FText> Dialogue;

	UPROPERTY(EditAnywhere)
	TArray<const TObjectPtr<ADaggerTrap>> DaggerTraps;

	UPROPERTY()
	FText Glyph;


	UFUNCTION()
	void OnAlarmTriggered(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void Reset();

	void BeginDestroy() override;

	UFUNCTION()
	const TArray<FText>& GetDialogue() const override;

	UFUNCTION() FORCEINLINE
	FSlateFontInfo& GetFontInfo() {return DialogueFont;}

	UFUNCTION() FORCEINLINE
	FText GetGlyph() const override {return Glyph;} 
	
	UFUNCTION()
	void DeactivateSecurityAlarm();

protected:
	virtual void BeginPlay() override;

private:
	TSharedPtr<FStreamableHandle> StreamableKeyCopyHandle;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Appearance;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* ActorRoot;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildActorComponent;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	bool bActiveMechanism = false;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	bool bCeilingDeactivated = false;

	UPROPERTY()
	FTimerHandle SpawnObjectHandle;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<class AKeyCopies> KeyCopyClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> KeyCopy = nullptr;


	template<typename T>  
	AActor* SpawnObject(TSubclassOf<T> Object);

	UFUNCTION()
	void SpawnKey(TSoftClassPtr<class AKeyCopies> Copy);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
