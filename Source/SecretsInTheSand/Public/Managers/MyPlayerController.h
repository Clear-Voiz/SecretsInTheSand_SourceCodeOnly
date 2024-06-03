#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerControllerDeath,FVector);


UCLASS()
class SECRETSINTHESAND_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DialoguesMappingContext;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShakeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USoundSubmix> MusicSubmix;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USoundSubmix> SFXSubmix;

	UPROPERTY()
	int32 DeactivatedAlarms = 0;
	
	FPlayerControllerDeath OnPlayerControllerDeath;
	

	
	UFUNCTION()
	void UpdateKeysHUD(int DeltaAmount);

	UFUNCTION()
	void UpdateScores(const int32& Amount);

	UFUNCTION()
	void AddDialogueHUD(const TArray<FText> &Dialogue, FSlateFontInfo& FontInfo, FText Glyph = FText::GetEmpty());

	UFUNCTION()
	void RemoveDialogueHUD();

	UFUNCTION()
	void AddGlyphsInputWidget();

	UFUNCTION()
	void RemoveGlyphsInputWidget();

	UFUNCTION()
	void AddMilestoneWidget();

	UFUNCTION()
	void RemoveMilestoneWidget();

	UFUNCTION(client, reliable)
	void ClientRPC_Respawn(const FTransform& RespawnTrans);

	UFUNCTION()
	void DisablePlayerCharacter(bool bConfirmation);


	UFUNCTION()
	void EndRumbling();

	UFUNCTION(BlueprintImplementableEvent)
	void RevealItself();

	UFUNCTION(server, reliable)
	void ServerRPC_CheckGlyphs(class AGlyphsManager* GlyphsManager, const FText& ReceivedCombination);

	UFUNCTION(server, reliable)
	void ServerRPC_DeactivateSecurityAlarm(class ASecurityAlarm* InteractedSecurityAlarm);

	UFUNCTION()
	void AddErrorMessage(FText Message);

	UFUNCTION()
	void AddWinWidget();

	UFUNCTION()
	class AGameHUD* GetGameHUD();

	UFUNCTION()
	int32 GetFinalScore();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void ReceiveAlarmDeactivationSignal();

	UFUNCTION()
	void StartRumbling();


private:
	UPROPERTY(EditAnywhere, Category = "Special Effects")
	TObjectPtr<class USoundBase> TheEgyptiansSFX;

	UPROPERTY(EditAnywhere, Category = "Special Effects")
	TObjectPtr<class USoundBase> TrembleSFX;
	

};
