#include "Managers/MyPlayerController.h"
#include "Managers/GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/GameModeAdministator.h"
#include "InputMappingContext.h"
#include "InteractableActors/SecurityAlarm.h"
#include "Managers/LTA_GameStateBase.h"
#include "SubManagers/GlyphsManager.h"
#include "Sound/SoundSubmix.h"
#include "InteractableActors/PointsPrize.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (!HasAuthority()) return;
    check(CameraShakeType);
    TArray<AActor*> SecurityAlarms;
    UGameplayStatics::GetAllActorsOfClass(this, ASecurityAlarm::StaticClass(),SecurityAlarms);

    for(AActor* SecurityAlarm : SecurityAlarms)
    {
        ASecurityAlarm* SA = Cast<ASecurityAlarm>(SecurityAlarm);
        if (!SA) continue;
        SA->OnActivated.AddUniqueDynamic(this, &AMyPlayerController::StartRumbling);
        SA->OnDeactivated.AddUniqueDynamic(this, &AMyPlayerController::EndRumbling);
        SA->OnDeactivated.AddUniqueDynamic(this, &AMyPlayerController::ReceiveAlarmDeactivationSignal);
    }

}

void AMyPlayerController::ReceiveAlarmDeactivationSignal()
{
    if (!HasAuthority()) return;
    if (++DeactivatedAlarms < 2) return;
    
    if (IGameModeAdministator* GameAdmin = Cast<IGameModeAdministator>(GetWorld()->GetAuthGameMode()))
        GameAdmin->ClearCommands();
    
}

void AMyPlayerController::UpdateKeysHUD(int DeltaAmount)
{
    if (AGameHUD* GameHUD = GetGameHUD())
    {
        GameHUD->CallUpdateKeys(DeltaAmount);
    }
}

void AMyPlayerController::UpdateScores(const int32 &Amount)
{
    if (AGameHUD* GameHUD = GetGameHUD()) GameHUD->UpdateScore(Amount);
}

void AMyPlayerController::AddDialogueHUD(const TArray<FText> &Dialogue, FSlateFontInfo& FontInfo, FText Glyph)
{
    if (AGameHUD* GameHUD = GetGameHUD())
    {
        GameHUD->StartDialogue(Dialogue, FontInfo, Glyph);
    }
}

void AMyPlayerController::RemoveDialogueHUD()
{
    if (AGameHUD* GameHUD = GetGameHUD())
    {
        GameHUD->EndDialogue();
    }
}

void AMyPlayerController::AddGlyphsInputWidget()
{
    if (AGameHUD* GameHUD = GetGameHUD())
    {
        GameHUD->AddGlyphsInputWidget();
    }
}

void AMyPlayerController::RemoveGlyphsInputWidget()
{
    if (AGameHUD* GameHUD = GetGameHUD())
    {
        GameHUD->RemoveGlyphsInputWidget();
    }
}

void AMyPlayerController::AddErrorMessage(FText Message)
{
    if (AGameHUD* GameHUD = GetGameHUD())
    {
        GameHUD->AddErrorMessage(Message);
    }
}

void AMyPlayerController::AddWinWidget()
{
    if (AGameHUD* GameHUD = GetGameHUD())
    {
        GameHUD->AddWinScreen(GetFinalScore());
        FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
    }
}

AGameHUD *AMyPlayerController::GetGameHUD()
{
    return Cast<AGameHUD>(GetHUD());
}

int32 AMyPlayerController::GetFinalScore()
{
    TArray<AActor*> Gems;
    UGameplayStatics::GetAllActorsOfClass(this, APointsPrize::StaticClass(),Gems);
    float FinalScore = 0;
    int32 TotalGems = 0;
    for (AActor* Gem : Gems)
    {
        APointsPrize* Points = CastChecked<APointsPrize>(Gem);
        TotalGems += Points->ScoreValue;
    }
    if (ALTA_GameStateBase* GSB = GetWorld()->GetGameState<ALTA_GameStateBase>())
    {
        int32 CurrentScore = GSB->GetScore();
        if (CurrentScore == 0) 
        {
            FinalScore = 0.f;
        }
        else
        {
            FinalScore = (CurrentScore/TotalGems) * 10;
            FinalScore = FMath::Floor(FinalScore);
        }
    }

    return (int)FinalScore;
}

void AMyPlayerController::ClientRPC_Respawn_Implementation(const FTransform& RespawnTrans)
{
    if (TheEgyptiansSFX) UGameplayStatics::PlaySound2D(this, TheEgyptiansSFX);
    ALuredToAdventureCharacter* MC = Cast<ALuredToAdventureCharacter>(GetPawn());
    if (MC) MC->Respawn(RespawnTrans);
    if (AGameHUD* GameHUD = GetGameHUD()) GameHUD->AddGameOverScreen();
    DeactivatedAlarms = 0;
    MusicSubmix->SetSubmixOutputVolume(this, 0.f);
}

void AMyPlayerController::DisablePlayerCharacter(bool bConfirmation)
{
    ALuredToAdventureCharacter* MC = Cast<ALuredToAdventureCharacter>(GetPawn());
    if (MC) MC->DisableCharacter(bConfirmation);
    SetIgnoreMoveInput(bConfirmation);
}

void AMyPlayerController::StartRumbling()
{
    if (!HasAuthority()) return;
    ClientStartCameraShake(CameraShakeType);
    
    if (!TrembleSFX) return;
    AActor* MC = GetPawn();
    ALTA_GameStateBase* GSB = GetWorld()->GetGameState<ALTA_GameStateBase>();

    if (!MC || ! GSB) return;
    GSB->MulticastRPC_SpawnSoundAtLocation(TrembleSFX, MC->GetActorLocation());
}

void AMyPlayerController::EndRumbling()
{
    if (!HasAuthority()) return;
    ClientStopCameraShake(CameraShakeType);

}

void AMyPlayerController::ServerRPC_CheckGlyphs_Implementation(AGlyphsManager* GlyphsManager, const FText& ReceivedCombination)
{
    GlyphsManager->CheckGlyphs(ReceivedCombination);
}

void AMyPlayerController::AddMilestoneWidget()
{
    if (AGameHUD* GameHUD = GetGameHUD())
    {
        GetPawn()->DisableInput(this);
        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputMode);
        
        GameHUD->AddMilestoneWidget();
    }
}

void AMyPlayerController::RemoveMilestoneWidget()
{
    if (AGameHUD* GameHUD = GetGameHUD())
    {
        SetIgnoreMoveInput(false);
        GetPawn()->EnableInput(this);
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
        GameHUD->RemoveMilestoneWidget();
    }
}


void AMyPlayerController::ServerRPC_DeactivateSecurityAlarm_Implementation(ASecurityAlarm* InteractedSecurityAlarm)
{
    InteractedSecurityAlarm->DeactivateSecurityAlarm();
}
