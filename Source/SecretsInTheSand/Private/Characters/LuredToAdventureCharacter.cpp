// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/LuredToAdventureCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GroomComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/Interactable.h"
#include "Managers/MyPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Widgets/InvestigationWidget.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "Managers/GameHUD.h"
#include "Managers/LTA_GameStateBase.h"
#include "Interfaces/Lethal.h"
#include "Components/BoxComponent.h"
#include "Managers/LuredToAdventureGameMode.h"
#include "InteractableActors/CheckableActor.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ALuredToAdventureCharacter. Called like that because the project name was previously LuredToAdventure


ALuredToAdventureCharacter::ALuredToAdventureCharacter()
{

	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 	
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	VisionDetectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vision Detection"));
	VisionDetectionMesh->SetupAttachment(GetRootComponent());
	VisionDetectionMesh->SetComponentTickEnabled(false);

	InvestigationWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Investigation Widget"));
	InvestigationWidget->SetupAttachment(GetRootComponent());

	TeamActionDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("Team Action Detection"));
	TeamActionDetection->SetupAttachment(GetRootComponent());

	PhysicsConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint"));
	PhysicsConstraintComponent->SetupAttachment(GetRootComponent());
	PhysicsConstraintComponent->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited,100.f);
	PhysicsConstraintComponent->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 100.f);
	PhysicsConstraintComponent->ConstraintInstance.ProfileInstance.bDisableCollision = true;

	if (HasAuthority())
	OnCharacterDeath.AddLambda([this](FVector DeathLoc){
		ALuredToAdventureGameMode* GameMode = GetWorld()->GetAuthGameMode<ALuredToAdventureGameMode>();
		if (GameMode) GameMode->Respawn(DeathLoc);});
}

void ALuredToAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager)
	{
		CameraManager->ViewPitchMax = 10.f;
		CameraManager->ViewPitchMin = -50.f;
	}
	
	SetDetection(false, VisionDetectionMesh);
	VisionDetectionMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALuredToAdventureCharacter::OnVisionBeginOverlap);
	VisionDetectionMesh->OnComponentEndOverlap.AddUniqueDynamic(this, &ALuredToAdventureCharacter::OnVisionEndOverlap);

	TeamActionDetection->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALuredToAdventureCharacter::OnTeamMemberBeginOverlapp);
	TeamActionDetection->SetGenerateOverlapEvents(false);

	RespawnLocation = GetActorLocation();

	InitialCameraBoomLength = GetCameraBoom()->TargetArmLength;

	check(ComboJumpMontage);
	check(ComboJumpAction);
	check(CarryingMappingContext);
	check(ImpulseSFX);

}

void ALuredToAdventureCharacter::ClientRPC_CallPlayCameraAnimation_Implementation()
{
	PlayCameraAnimation();
}

void ALuredToAdventureCharacter::OnVisionBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (IInteractable* Checkable = Cast<IInteractable>(OtherActor))
	{
		bCanInteract = true;
		SetInvestigationWidgetIndex(1);
		
	}
}

void ALuredToAdventureCharacter::OnVisionEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (IInteractable* Checkable = Cast<IInteractable>(OtherActor))
	{
		bCanInteract = false;
		SetInvestigationWidgetIndex(0);
	}
}


void ALuredToAdventureCharacter::SetDetection(bool bCanDetect, UPrimitiveComponent* TargetComponent)
{
	TargetComponent->SetGenerateOverlapEvents(bCanDetect);
}

void ALuredToAdventureCharacter::SetInvestigationWidgetVisibility(bool bIsHidden)
{
	InvestigationWidget->SetHiddenInGame(bIsHidden);
}

void ALuredToAdventureCharacter::SetInvestigationWidgetIndex(int32 index)
{
	UInvestigationWidget* QuestionIcon = Cast<UInvestigationWidget>(InvestigationWidget->GetWidget());
	if (!QuestionIcon) return;
	QuestionIcon->SwitchWidget(index);

}

void ALuredToAdventureCharacter::Rooter()
{
    bFinishedLanding = true;
}

void ALuredToAdventureCharacter::SwitchMappingContext(const UInputMappingContext* OldMappingContext, const UInputMappingContext* NewMappingContext)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (Subsystem->HasMappingContext(NewMappingContext)) return;
			Subsystem->AddMappingContext(NewMappingContext, 0);
			CurrentMappingContext = NewMappingContext;
			if (Subsystem->HasMappingContext(OldMappingContext)) Subsystem->RemoveMappingContext(OldMappingContext);
		}
	}
}

AMyPlayerController *ALuredToAdventureCharacter::GetMyPlayerController() const
{
	return Cast<AMyPlayerController>(GetController());
}

void ALuredToAdventureCharacter::DisableCharacter(bool bConfirmation)
{
	RevealItself();
	SetActorTickEnabled(!bConfirmation);
}

void ALuredToAdventureCharacter::Respawn(const FTransform& RespawnTrans)
{
	SetDetection(false,VisionDetectionMesh);
	SetInvestigationWidgetIndex(0);
	SetInvestigationWidgetVisibility(true);
	if (AController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetIgnoreMoveInput(true);
	}
	if (bCarryState)
	{
		SetMovementLimits(500.f,600.f);
		bCarryState = false;
	}
	if (Interactor) Interactor = nullptr;
	SwitchMappingContext(CurrentMappingContext, DefaultMappingContext);

	ClientRPC_StopSpawnedSounds();
}

void ALuredToAdventureCharacter::UpdateCheckpoint(FVector NewLoc)
{
	RespawnLocation = NewLoc;
}

void ALuredToAdventureCharacter::SetCharacterHairColors(UMaterialInterface* HairMat, UMaterialInterface* EyebrowsMat)
{	
	if (!HairMat || !EyebrowsMat) return;
	Hair->SetMaterial(0,HairMat);
	Eyebrows->SetMaterial(0,EyebrowsMat);
}

void ALuredToAdventureCharacter::ClientRPC_StopSpawnedSounds_Implementation()
{
	if (ALTA_GameStateBase* GS = GetWorld()->GetGameState<ALTA_GameStateBase>())
	{
		if (GS->CurrentSounds.IsEmpty()) return;
		for(TObjectPtr<UAudioComponent> AudioComponent : GS->CurrentSounds)
		{
			if (!AudioComponent) continue;
			if (AudioComponent->IsPlaying())
			{
				AudioComponent->Stop();
			}
		}
		GS->CurrentSounds.Empty();
	}
}

void ALuredToAdventureCharacter::Landed(const FHitResult &Hit)
{
	bFinishedLanding = false;
	GetWorldTimerManager().SetTimer(RooterHandler, this, &ALuredToAdventureCharacter::Rooter,Delay);
}

void ALuredToAdventureCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALuredToAdventureCharacter, bCanComboJump);
}

void ALuredToAdventureCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALuredToAdventureCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALuredToAdventureCharacter::Look);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ALuredToAdventureCharacter::Interact);

		EnhancedInputComponent->BindAction(ProgressInteractionAction, ETriggerEvent::Triggered, this, &ALuredToAdventureCharacter::ProgressInteraction);

		EnhancedInputComponent->BindAction(FinishInteractionAction, ETriggerEvent::Triggered, this, &ALuredToAdventureCharacter::EndInteraction);
	
		EnhancedInputComponent->BindAction(ComboJumpAction,ETriggerEvent::Triggered, this, &ALuredToAdventureCharacter::ComboJump);

		EnhancedInputComponent->BindAction(IA_CancelComboJump,ETriggerEvent::Triggered, this, &ALuredToAdventureCharacter::CancelComboJump);
	}
	
}


void ALuredToAdventureCharacter::Move(const FInputActionValue& Value)
{
	if(!bFinishedLanding) return;
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALuredToAdventureCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ALuredToAdventureCharacter::Interact(const FInputActionValue &Value)
{	
	if (!bCanInteract) return;
	
	AMyPlayerController* MyPlayerController = GetMyPlayerController();
	if (!MyPlayerController) return;

	TArray<AActor*> OverlappingActors;
	VisionDetectionMesh->GetOverlappingActors(OverlappingActors);
	IInteractable* Interactable;
	for (AActor* OverlappedActor : OverlappingActors)
	{
		if (OverlappedActor->Implements<UInteractable>())
		{
			Interactable = CastChecked<IInteractable>(OverlappedActor);
			Interactable->Interact(this);
			break;
		}
	}
}

void ALuredToAdventureCharacter::MulticastRPC_PlayMontage_Implementation(UAnimMontage *Montage, FName Section)
{
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance || !Montage) return;
    AnimInstance->Montage_Play(Montage);
	if (Section.IsNone()) return;
	AnimInstance->Montage_JumpToSection(Section,Montage);
}

void ALuredToAdventureCharacter::ServerRPC_PlayMontage_Implementation(UAnimMontage *Montage, FName Section)
{
	MulticastRPC_PlayMontage(Montage, Section);
}

void ALuredToAdventureCharacter::CancelComboJump()
{
	ServerRPC_PlayMontage(ComboJumpMontage,"Stand_Up");
}
void ALuredToAdventureCharacter::OnTeamMemberBeginOverlapp(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!HasLocalNetOwner()) return;
	if (OtherActor == this || !OtherActor->IsA(ALuredToAdventureCharacter::StaticClass())) return;
	ServerRPC_PlayMontage(ComboJumpMontage,"Launch_Character");
	ServerRPC_OnCharacterLaunch(OtherActor);
}

void ALuredToAdventureCharacter::ServerRPC_OnCharacterLaunch_Implementation(AActor* Launched)
{
	FVector JumpOrigin = TeamActionDetection->GetComponentLocation();
	Launched->SetActorLocation(JumpOrigin);
	ALuredToAdventureCharacter* LC = Cast<ALuredToAdventureCharacter>(Launched);

	if (!LC) return;
	LC->ClientRPC_CallPlayCameraAnimation();
	ClientRPC_CallPlayCameraAnimation();

	UCharacterMovementComponent* OtherMovementComp = LC->GetCharacterMovement();
	FVector LaunchDirection = LC->GetActorForwardVector();
	double Radians = UKismetMathLibrary::DegreesToRadians(75.f);
	LaunchDirection.Z = sin(Radians);
	LaunchDirection.X *= cos(Radians);
	LaunchDirection.Y *= cos(Radians);
	LC->LaunchCharacter(LaunchDirection * 1150.f,true,true);
	LC->ServerRPC_PlayMontage(LC->ComboJumpMontage, "On_Air");

	if (!ImpulseVFX) return;
	if (ALTA_GameStateBase* GSB = GetWorld()->GetGameState<ALTA_GameStateBase>())
		{
			GSB->MulticastRPC_SpawnCascadeAtLocation(ImpulseVFX, TeamActionDetection->GetComponentLocation());
			if (ImpulseSFX) GSB->MulticastRPC_PlaySoundAtLocation(ImpulseSFX, GetActorLocation());
		}
}

void ALuredToAdventureCharacter::ProgressInteraction(const FInputActionValue &Value)
{
	AGameHUD* GameHUD = Cast<AGameHUD>(GetMyPlayerController()->GetHUD());
	if (GameHUD) GameHUD->UpdateDialogue();
}

void ALuredToAdventureCharacter::ComboJump(const FInputActionValue &Value)
{
	if (GetMovementComponent()->IsFalling() || !bCanComboJump) return;
	ServerRPC_PlayMontage(ComboJumpMontage);
	SwitchMappingContext(DefaultMappingContext,MC_ComboJump);
}

void ALuredToAdventureCharacter::EndInteraction()
{
	if (!Interactor) return;
	if (Interactor->Implements<UInteractable>())
	{
		CastChecked<IInteractable>(Interactor)->EndInteraction(this);
	}
}

void ALuredToAdventureCharacter::ServerRPC_EnterCarryState_Implementation(AActor* InteractionSource)
{
	MulticastRPC_AdjustCarryParameters(InteractionSource);
}

void ALuredToAdventureCharacter::ServerRPC_ExitCarryState_Implementation()
{
	MulticastRPC_AdjustFreehandsParameters();
}

void ALuredToAdventureCharacter::MulticastRPC_AdjustCarryParameters_Implementation(AActor* InteractionSource)
{
	Interactor = InteractionSource;
	bCarryState = true;
	if (!InteractionSource->IsA(ACheckableActor::StaticClass())) return;
	UPrimitiveComponent* ObjectHandle = Cast<UStaticMeshComponent>(InteractionSource->GetRootComponent()); //GetRootComponent());
    ObjectHandle->SetSimulatePhysics(false);
	ObjectHandle->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
	ObjectHandle->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
    UCharacterMovementComponent* MovementComp = GetCharacterMovement();
    MovementComp->MaxWalkSpeed = 200.f;
    MovementComp->JumpZVelocity = 100.f;

	if (ACheckableActor* CheckableActor = Cast<ACheckableActor>(Interactor))
	{
		UBoxComponent* DetectionShape =  CheckableActor->GetDetectionShape();
		if (DetectionShape) DetectionShape->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
	}

	if (!HasAuthority()) return;
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,false);
    bool bDidAttach = InteractionSource->AttachToComponent(GetMesh(),TransformRules,FName("hand_rSocket"));//Owner->AttachToActor(Subject,TransformRules,FName("hand_rSocket"));
}

void ALuredToAdventureCharacter::MulticastRPC_AdjustFreehandsParameters_Implementation()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
    MovementComp->MaxWalkSpeed = 500.f;
    MovementComp->JumpZVelocity = 600.f;
	bCarryState = false;
	
	ReturnGravityToBox();

	UPrimitiveComponent* ObjectHandle = Cast<UPrimitiveComponent>(Interactor->GetRootComponent());
	ObjectHandle->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Block);
	ObjectHandle->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Block);
	

	if (!HasAuthority())
	{
		Interactor = nullptr;
		return;
	} 
    FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld,true);
    Interactor->DetachFromActor(DetachmentRules);
    FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * 200.f);
    Interactor->SetActorLocation(NewLocation);
    FRotator CurrentRotation = Interactor->GetActorRotation();
    FRotator NewRotation(0.f, CurrentRotation.Yaw, 0.f);
    Interactor->AddActorLocalRotation(NewRotation);	
	Interactor = nullptr;

}

void ALuredToAdventureCharacter::ReturnGravityToBox()
{
	if (!Interactor) return;
	if (ACheckableActor* CheckableActor = Cast<ACheckableActor>(Interactor))
	{
		UBoxComponent* DetectionShape =  CheckableActor->GetDetectionShape();
		if (DetectionShape) DetectionShape->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	}
	Cast<UPrimitiveComponent>(Interactor->GetRootComponent())->SetSimulatePhysics(true);
}

void ALuredToAdventureCharacter::SetMovementLimits(float MaxWalkSpeed, float JumpZVelocity)
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->MaxWalkSpeed = MaxWalkSpeed;
    	CharacterMovementComponent->JumpZVelocity = JumpZVelocity;
	}
	
}
