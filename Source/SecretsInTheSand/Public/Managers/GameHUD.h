#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"


UCLASS()
class SECRETSINTHESAND_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> GameUIClass;

	UPROPERTY(VisibleAnywhere)
	class UGameWidget* GameUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDialogueWidget> DialogueClass;

	UPROPERTY(VisibleAnywhere)
	class UDialogueWidget* DialogueWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGameOverWidget> GameOverWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameOverWidget> GameOverClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UHieroglyphsInputWidget> GlyphsClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UMilestoneWidget> MilestoneWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMilestoneWidget> MilestoneClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCJumpGuideWidget> CJumpGuideWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCJumpGuideWidget> CJumpGuideClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UErrorMessageWidget> ErrorMessageWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UErrorMessageWidget> ErrorMessageClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWinScreen> WinScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UWinScreen> WinScreenClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStartingGuideWidget> StartingGuideWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UStartingGuideWidget> StartingGuideClass;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle RemoveErrorHandle;



	UFUNCTION()
	void CallUpdateKeys(int DeltaAmmount);

	UFUNCTION()
	void StartDialogue(const TArray<FText>& Dialogue, FSlateFontInfo& FontInfo, FText Glyph);

	UFUNCTION()
	void UpdateDialogue();

	UFUNCTION()
	void EndDialogue();

	UFUNCTION()
	void UpdateScore(const int32& DeltaAmmount);

	UFUNCTION()
	void AddGameOverScreen();

	UFUNCTION()
	void AddGlyphsInputWidget();

	UFUNCTION()
	void RemoveGlyphsInputWidget();

	UFUNCTION() FORCEINLINE
	class UHieroglyphsInputWidget* GetGlyphsWidget() const {return GlyphsWidget;}

	UFUNCTION()
	void AddMilestoneWidget();

	UFUNCTION()
	void RemoveMilestoneWidget();

	UFUNCTION()
	void AddCJumpGuideWidget();

	UFUNCTION()
	void AddErrorMessage(FText Message);

	UFUNCTION()
	void RemoveErrorMessage();

	UFUNCTION()
	void AddWinScreen(int32 FinalResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UHieroglyphsInputWidget> GlyphsWidget;

};
