#include "Traps/CrocoEnemy.h"
#include "Components/BoxComponent.h"
#include "Managers/CrocoAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TriggerVolume.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/Death.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/LTA_GameStateBase.h"

ACrocoEnemy::ACrocoEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	CrocoLimits = CreateDefaultSubobject<UBoxComponent>(TEXT("Detection Area"));
	CrocoLimits->SetupAttachment(GetRootComponent());

}

void ACrocoEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;
	if (!PreyDetectionArea) return;
	PreyDetectionArea->OnActorBeginOverlap.AddDynamic(this, &ACrocoEnemy::OnCharacterBeginDetection);
	PreyDetectionArea->OnActorEndOverlap.AddDynamic(this, &ACrocoEnemy::OnCharacterEndDetection);
	CrocoLimits->OnComponentBeginOverlap.AddDynamic(this, &ACrocoEnemy::OnHitboxBeginOverlap);
}

void ACrocoEnemy::OnCharacterBeginDetection(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!HasAuthority()) return;
	if (!OtherActor->IsA(ACharacter::StaticClass()) || OtherActor == this) return;
	PerceivedPreys.Add(CastChecked<ACharacter>(OtherActor));
	ACrocoAIController* CrocoAI = GetController<ACrocoAIController>();
	if (CrocoAI) CrocoAI->GetBlackboardComponent()->SetValueAsObject(TEXT("Prey"),PerceivedPreys[0]);
	OnPreyDetected.ExecuteIfBound(ECrocoStates::EEE_Chase);
}

void ACrocoEnemy::OnCharacterEndDetection(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!HasAuthority()) return;
	if (!OtherActor->IsA(ACharacter::StaticClass()) || OtherActor == this) return;
	PerceivedPreys.Remove(CastChecked<ACharacter>(OtherActor));
	ACrocoAIController* CrocoAI = GetController<ACrocoAIController>();
	if (!CrocoAI) return;
	if (PerceivedPreys.Num() > 0)
	{
		CrocoAI->GetBlackboardComponent()->SetValueAsObject(TEXT("Prey"),PerceivedPreys[0]);
		OnPreyDetected.ExecuteIfBound(ECrocoStates::EEE_Chase);}
	else
	{
		CrocoAI->GetBlackboardComponent()->ClearValue(TEXT("Prey"));
		OnPreyDetected.ExecuteIfBound(ECrocoStates::EEE_ReturnToOrigins);
	}
}

void ACrocoEnemy::OnHitboxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!OtherActor->IsA(ACharacter::StaticClass()) || OtherActor == this) return;
	ACrocoAIController* CrocoAI = GetController<ACrocoAIController>();
	if (!CrocoAI) return;
	
	PlaySoundAtLocation(SplashSFX, this);
	PlaySoundAtLocation(BiteSFX, CastChecked<ACharacter>(OtherActor));

	if (SweepResult.ImpactNormal.Z < 0)
	{
		CrocoAI->SetCurrentState(ECrocoStates::EEE_UpBite);
		FRotator CurrentRotation = GetActorRotation();
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),SweepResult.ImpactPoint);
		FRotator NewRotation(CurrentRotation.Pitch, LookAtRot.Yaw, CurrentRotation.Roll);
		SetActorRotation(NewRotation);
	} 
	else
	{
		CrocoAI->SetCurrentState(ECrocoStates::EEE_ForwardBite);
	}
	
}
void ACrocoEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACrocoEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACrocoEnemy,CurrentState);
}

void ACrocoEnemy::EatPrey()
{
	if (!HasAuthority() || PerceivedPreys.Num() < 1 || PerceivedPreys[0] == nullptr) return;
	if (PerceivedPreys[0]->Implements<UDeath>())
	{
		CastChecked<IDeath>(PerceivedPreys[0])->OnCharacterDeath.Broadcast(PerceivedPreys[0]
		->GetActorLocation());
	}
}

void ACrocoEnemy::PlaySoundAtLocation(USoundBase* SFX, ACharacter* SoundPlayer)
{
	if (!SFX) return;
	ALTA_GameStateBase* GSB = GetWorld()->GetGameState<ALTA_GameStateBase>();
	if (GSB)
	{
		GSB->MulticastRPC_PlaySoundAtLocation(SFX,SoundPlayer->GetActorLocation());
	}
}
