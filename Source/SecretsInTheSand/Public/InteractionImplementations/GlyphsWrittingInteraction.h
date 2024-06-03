#pragma once

#include "CoreMinimal.h"
#include "InteractionImplementations/BasicInteraction.h"
#include "GlyphsWrittingInteraction.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class SECRETSINTHESAND_API UGlyphsWrittingInteraction : public UBasicInteraction
{
	GENERATED_BODY()

public:
	void ImplementInteraction(class ALuredToAdventureCharacter* Subject, AActor* Owner = nullptr) override;
	void ImplementEndInteraction(class ALuredToAdventureCharacter* Subject, AActor* Owner = nullptr) override;

	UFUNCTION() FORCEINLINE
	void SetGlyphsManager(AGlyphsManager* WorldGlyphsManager) {GlyphsManager = WorldGlyphsManager;}

protected:
	virtual void BeginPlay() override;
	void LookAtObject(class ALuredToAdventureCharacter* Subject) override;
	
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ALuredToAdventureCharacter> MC;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<class AGlyphsManager> GlyphsManager;

	UPROPERTY()
	TObjectPtr<class UHieroglyphsInputWidget> GlyphsWidget;

	UFUNCTION()
	void RemoveGlyphsInputWidget();


};
