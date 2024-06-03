// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/GameModeAdministator.h"
#include "LuredToAdventureGameMode.generated.h"

UCLASS(minimalapi)
class ALuredToAdventureGameMode : public AGameModeBase, public IGameModeAdministator
{
	GENERATED_BODY()

public:
	ALuredToAdventureGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void InitGameState() override;

	UPROPERTY()
	TArray<TObjectPtr<class AMyPlayerController>> PlayerControllers;

	UPROPERTY()
	TArray<AActor*> Characters;

	UPROPERTY()
	TArray<TObjectPtr<class UCommand>> PendingCommands;

	UPROPERTY(EditAnywhere, Category = "Character Variations")
	TSubclassOf<class ACharacter> BlueCharLook;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACharacter> CurrentCharToAssign;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACharacter> RedCharLook;

	

	UFUNCTION()
	void UpdateScore(const int32& Amount);

	void AddCommandAndUpdateScore(UClass* ItemType, const int32& Amount, const FVector& Location) override;

	void AddCommand(class ASecurityAlarm* Alarm) override;

	void ClearCommands() override;

	UFUNCTION(BlueprintImplementableEvent)
	void RevealItself();

	UFUNCTION()
	void Respawn(FVector RespawnLoc);

	UFUNCTION()
	void SetCharactersVisibility(bool bHide);

	UFUNCTION() FORCEINLINE
	TArray<AActor*> GetCharacters() const {return Characters;}

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	UFUNCTION()
	void UnlockComboJump() override;

	UFUNCTION()
	void StopSpawnedSounds();


};



