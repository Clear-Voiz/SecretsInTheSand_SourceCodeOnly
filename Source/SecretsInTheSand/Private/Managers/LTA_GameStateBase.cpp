#include "Managers/LTA_GameStateBase.h"
#include "Managers/MyPlayerController.h"
#include "Managers/LuredToAdventureGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"


void ALTA_GameStateBase::UpdateScore(const int32 &EarnedPoints)
{
    Score += EarnedPoints;
    OnRep_Score(GetScore());
}

void ALTA_GameStateBase::OnRep_Score(const int32 &EarnedPoints) 
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0);// PlayerArray[0]->GetPlayerController();
    if (!PlayerController) return;
    AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController);
    if (MyPlayerController) MyPlayerController->UpdateScores(GetScore());
}

void ALTA_GameStateBase::PostCinematic()
{

    if (AMyPlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
    {
        MyPlayerController->DisablePlayerCharacter(false);
    }
    if (!HasAuthority()) return;
    if (ALuredToAdventureGameMode* MyGameMode = GetWorld()->GetAuthGameMode<ALuredToAdventureGameMode>())
        MyGameMode->SetCharactersVisibility(false);
    
        
}

void ALTA_GameStateBase::MulticastRPC_PlaySoundAtLocation_Implementation(USoundBase *SFX, FVector Location)
{
    if (!SFX) return;
    UGameplayStatics::PlaySoundAtLocation(this, SFX, Location);
}
void ALTA_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ALTA_GameStateBase, Score);
}

void ALTA_GameStateBase::MulticastRPC_SpawnSoundAtLocation_Implementation(class USoundBase* SFX, FVector Location)
{
    CurrentSounds.Add(UGameplayStatics::SpawnSoundAtLocation(this, SFX, Location));

}

void ALTA_GameStateBase::MulticastRPC_SpawnParticleAtLocation_Implementation(UNiagaraSystem* VFX, const FVector& SpawnLoc)
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, VFX, SpawnLoc);
}

void ALTA_GameStateBase::MulticastRPC_SpawnCascadeAtLocation_Implementation(class UParticleSystem* VFX, const FVector& SpawnLoc)
{
    UGameplayStatics::SpawnEmitterAtLocation(this, VFX, SpawnLoc);
}

void ALTA_GameStateBase::MulticastRPC_PlayCinematic_Implementation(ULevelSequence* SceneToPlay, ALevelSequenceActor* LevelSequenceActor)
{
    if (AMyPlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
        {
            MyPlayerController->DisablePlayerCharacter(true);
        }

    if (LevelSequenceActor && SceneToPlay)
	{
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this,SceneToPlay,PlaybackSettings,LevelSequenceActor);
		SequencePlayer->Play();
        float EndTime = SequencePlayer->GetEndTime().AsSeconds();
        FTimerDelegate CinematicsDelegate = FTimerDelegate::CreateUObject(this, &ALTA_GameStateBase::PostCinematic);
        GetWorld()->GetTimerManager().SetTimer(CinematicsHandle, CinematicsDelegate, EndTime, false);  
	}

    if (!HasAuthority()) return;
    if (ALuredToAdventureGameMode* MyGameMode = GetWorld()->GetAuthGameMode<ALuredToAdventureGameMode>())
        {
            MyGameMode->SetCharactersVisibility(true);
        }
}