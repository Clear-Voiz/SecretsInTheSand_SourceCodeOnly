#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/Interactable.h"
#include "CheckableActor.generated.h"

UCLASS()
class SECRETSINTHESAND_API ACheckableActor : public AActor, public IInteractable
{
	GENERATED_BODY()

private:


	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> RootComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Appearance;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> DetectionShape;

public:
	ACheckableActor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Interact(ALuredToAdventureCharacter* Subject) override;

	void EndInteraction(ALuredToAdventureCharacter* Subject) override;

	UPROPERTY(EditAnywhere)
	TArray<FText> Dialogue;

	UPROPERTY(EditAnywhere, Replicated)
	FText Glyph;

	UPROPERTY(EditAnywhere)
	FSlateFontInfo DialogueFont;
	
	UPROPERTY(EditAnywhere, Category = Interaction)
	TSubclassOf<class UBasicInteraction> InteractionType;

	UPROPERTY(VisibleAnywhere, Category = Interaction)
	TObjectPtr<class UBasicInteraction> InteractionComponent;

	UFUNCTION() FORCEINLINE
	class UBoxComponent* GetDetectionShape() const {return DetectionShape;}

	UFUNCTION() FORCEINLINE
	class UStaticMeshComponent* GetAppearance() const {return Appearance;}

	UFUNCTION()
	const TArray<FText>& GetDialogue() const override;

	UFUNCTION() FORCEINLINE
	FSlateFontInfo& GetFontInfo() {return DialogueFont;}

	UFUNCTION() FORCEINLINE
	FText GetGlyph() const override {return Glyph;} 
};
