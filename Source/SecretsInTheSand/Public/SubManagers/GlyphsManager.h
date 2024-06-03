#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Triggerable.h"
#include "GlyphsManager.generated.h"

DECLARE_DELEGATE(FOnSolved);

UCLASS()
class SECRETSINTHESAND_API AGlyphsManager : public AActor, public ITriggerable
{
	GENERATED_BODY()
	
public:	
	AGlyphsManager();
	
	UPROPERTY(EditAnywhere)
	int32 GlyphsPerRow = 5;

	UPROPERTY(EditAnywhere)
	TArray<FString> InscriptionOptions;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class ACheckableActor>> InscriptionTables;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class ACheckableActor>> InscriptionCanvases;

	FOnSolved OnSolved;

	UPROPERTY(ReplicatedUsing = OnRep_ActivateMechanisms)
	bool bAlreadyActivated = false;

	UFUNCTION() FORCEINLINE
	TArray<FText> GetKeyGlyphs() const {return KeyGlyphs;}

	UFUNCTION()
	void ListenGlyphsInputWidget(class UHieroglyphsInputWidget* GlyphsWidget);

	UFUNCTION()
	void RemoveGlyphsInputWidget();
	
	UFUNCTION()
	void CheckGlyphs(const FText& ReceivedCombination);

	UFUNCTION() FORCEINLINE
	FText GetPassword() const {return Password;}

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere)
	TArray<FText> KeyGlyphs;

	UPROPERTY(VisibleAnywhere)
	FText Password;


	UFUNCTION()
	void AssignInscriptionsToTables();

	UFUNCTION()
	void SendServerCheckGlyphs(const FText& ReceivedCombination);

	UFUNCTION()
	void OnRep_ActivateMechanisms();

};
