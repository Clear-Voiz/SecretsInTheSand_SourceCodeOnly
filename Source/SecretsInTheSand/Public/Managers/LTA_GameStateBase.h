#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LTA_GameStateBase.generated.h"

DECLARE_DELEGATE_ThreeParams(FGameStatePointsCollected, UClass*, const int32&, const FVector&)


UCLASS()
class SECRETSINTHESAND_API ALTA_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	FGameStatePointsCollected OnGameStatePointsCollected;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UAudioComponent>> CurrentSounds;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const;

	UFUNCTION()
	FORCEINLINE int32 GetScore() const {return Score;}

	UFUNCTION()
	void UpdateScore(const int32& EarnedPoints);

	UFUNCTION()
	void OnRep_Score(const int32& EarnedPoints);

	UFUNCTION(netmulticast, reliable)
	void MulticastRPC_PlaySoundAtLocation(USoundBase* SFX, FVector Location);

	UFUNCTION(netmulticast, reliable)
	void MulticastRPC_SpawnSoundAtLocation(class USoundBase* SFX, FVector Location);

	UFUNCTION(netmulticast, unreliable)
	void MulticastRPC_SpawnParticleAtLocation(class UNiagaraSystem* VFX, const FVector& SpawnLoc);

	UFUNCTION(netmulticast, unreliable)
	void MulticastRPC_SpawnCascadeAtLocation(class UParticleSystem* VFX, const FVector& SpawnLoc);

	UFUNCTION(netmulticast, reliable)
	void MulticastRPC_PlayCinematic(class ULevelSequence* SceneToPlay, class ALevelSequenceActor* LevelSequenceActor);

	UFUNCTION()
	void PostCinematic();

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Score)
	int32 Score;


	UPROPERTY()
	FTimerHandle CinematicsHandle;


};
