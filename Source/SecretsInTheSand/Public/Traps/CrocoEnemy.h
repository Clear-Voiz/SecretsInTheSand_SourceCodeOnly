#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Patterns/ECrocoStates.h"
#include "CrocoEnemy.generated.h"


DECLARE_DELEGATE_OneParam(FOnCrocoEnemyPreyDetected, ECrocoStates);

UCLASS()
class SECRETSINTHESAND_API ACrocoEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ACrocoEnemy();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	ECrocoStates CurrentState = ECrocoStates::EEE_Idle;

	FOnCrocoEnemyPreyDetected OnPreyDetected;

	UPROPERTY(VisibleAnywhere)
	float Speed = 1.f;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<class ATriggerVolume> PreyDetectionArea;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void EatPrey();

	UFUNCTION()
	void PlaySoundAtLocation(class USoundBase* SFX, ACharacter* SoundPlayer);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CrocoLimits;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CrocoHitbox;

	TArray<TObjectPtr<ACharacter>> PerceivedPreys;

	UPROPERTY(EditAnywhere, Category = "Special Effects")
	TObjectPtr<class USoundBase> BiteSFX;

	UPROPERTY(EditAnywhere, Category = "Spcecial Effects")
	TObjectPtr<class USoundBase> SplashSFX;

	UFUNCTION()
	void OnCharacterBeginDetection(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnCharacterEndDetection(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
