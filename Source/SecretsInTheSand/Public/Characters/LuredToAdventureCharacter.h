// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interfaces/Death.h"
#include "Engine/TimerHandle.h"
#include "LuredToAdventureCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
class UAnimMontage;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DELEGATE_ThreeParams(FCharacterPointsCollected,UClass*, const int32&, const FVector&);

UCLASS(config=Game)
class ALuredToAdventureCharacter : public ACharacter, public IDeath
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DialoguesMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* CarryingMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputMappingContext> MC_ComboJump;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputMappingContext> MC_Null;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ProgressInteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FinishInteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ComboJumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_CancelComboJump;

	UPROPERTY(EditAnywhere, Category = "Interactions")
	TObjectPtr<class UPhysicsConstraintComponent> PhysicsConstraintComponent;

	UPROPERTY(EditAnywhere, Category = SFX)
	TObjectPtr<class USoundBase> ImpulseSFX;


	UFUNCTION()
	void CancelComboJump();

	UPROPERTY(visibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(visibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisionDetectionMesh;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* InvestigationWidget;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ComboJumpMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Team Actions", Meta = (AllowPrivateAccess="true"))
	TObjectPtr<class UBoxComponent> TeamActionDetection;

	UPROPERTY(VisibleAnywhere)
	bool bCanInteract = false;

	UPROPERTY(VisibleAnywhere, Replicated)
	bool bCanComboJump = false;

	UPROPERTY(EditAnywhere, Category = "Special Effects")
	TObjectPtr<class UParticleSystem> ImpulseVFX;


	UFUNCTION()
	void OnTeamMemberBeginOverlapp(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);	

	UFUNCTION(server, reliable)
	void ServerRPC_OnCharacterLaunch(AActor* Launched);

	UFUNCTION()
	void ReturnGravityToBox();

	UFUNCTION()
	void SetMovementLimits(float MaxWalkSpeed, float JumpZVelocity);

	const float Delay = .25f;

	FTimerHandle RooterHandler;

	FTimerHandle UnloadInteractorHandler;

	void Rooter();

	virtual void Landed(const FHitResult& Hit) override;
	
protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly)
	float InitialCameraBoomLength;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	UFUNCTION(netmulticast, reliable)
	void MulticastRPC_PlayMontage(UAnimMontage* Montage,FName Section = FName(NAME_None));


	void ProgressInteraction(const FInputActionValue &Value);

	void ComboJump(const FInputActionValue &Value);

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	virtual void BeginPlay();

	UFUNCTION()
	void OnVisionBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnVisionEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

public:
	ALuredToAdventureCharacter();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION() FORCEINLINE
	class UInputMappingContext* GetDefaultMappingContext() const { return DefaultMappingContext;}

	UFUNCTION() FORCEINLINE
	class UInputMappingContext* GetCarryingMappingContext() const { return CarryingMappingContext;}


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bFinishedLanding = true;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* CheckingMontage;
	
	UPROPERTY(VisibleAnywhere)
	AActor* Interactor;

	UPROPERTY(VisibleAnywhere)
	bool bCarryState = false;

	FCharacterPointsCollected OnCharacterPointsCollected;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> AltHairColor;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> AltEyebrowsColor;

	UPROPERTY(VisibleAnywhere, Replicated)
	bool bHasTreasure = false;

	UPROPERTY(VisibleAnywhere)
	const class UInputMappingContext* CurrentMappingContext;
	

	UFUNCTION(BlueprintImplementableEvent)
	void RevealItself();

	UFUNCTION(server, reliable)
	void ServerRPC_EnterCarryState(AActor* InteractionSource);

	UFUNCTION(server, reliable)
	void ServerRPC_ExitCarryState();

	UFUNCTION(netmulticast, reliable)
	void MulticastRPC_AdjustFreehandsParameters();

	UFUNCTION(netmulticast, reliable)
	void MulticastRPC_AdjustCarryParameters(AActor* InteractionSource);

	UFUNCTION()
	void SetDetection(bool bCanDetect, UPrimitiveComponent* TargetComponent);

	UFUNCTION()
	void SetInvestigationWidgetVisibility(bool bIsHidden);

	UFUNCTION()
	void SetInvestigationWidgetIndex(int32 index);

	UFUNCTION(BlueprintImplementableEvent)
	void SlerpToCheckableObject(FQuat CharacterPosition, FQuat LookAtRotation);

	void EndInteraction();
	
	UFUNCTION()
	void SwitchMappingContext(const UInputMappingContext* OldMappingContext, const UInputMappingContext* NewMappingContext);

	UFUNCTION()
	class AMyPlayerController* GetMyPlayerController() const;

	UFUNCTION()
	FORCEINLINE void SetCanInteract(const bool bState) {bCanInteract = bState;}

	UFUNCTION(BlueprintCallable)
	void DisableCharacter(bool bConfirmation);

	UFUNCTION()
	void Respawn(const FTransform& RespawnTrans);

	UFUNCTION()
	void UpdateCheckpoint(FVector NewLoc) override;

	UFUNCTION()
	FORCEINLINE UStaticMeshComponent* GetVisionDetectionMesh() const {return VisionDetectionMesh;}

	UFUNCTION()
	FORCEINLINE UBoxComponent* GetTeamActionDetection() const {return TeamActionDetection.Get();}

	UFUNCTION(server, reliable)
	void ServerRPC_PlayMontage(UAnimMontage* Montage, FName Section = FName(NAME_None));

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCameraAnimation();

	UFUNCTION(client, unreliable)
	void ClientRPC_CallPlayCameraAnimation();

	UFUNCTION()
	void SetCharacterHairColors(UMaterialInterface* HairMat, UMaterialInterface* EyebrowsMat);

	UFUNCTION() FORCEINLINE
	void SetCanComboJump(bool bIsUnlocked) {bCanComboJump = bIsUnlocked;}
	
	UFUNCTION(client, reliable)
	void ClientRPC_StopSpawnedSounds();



};

