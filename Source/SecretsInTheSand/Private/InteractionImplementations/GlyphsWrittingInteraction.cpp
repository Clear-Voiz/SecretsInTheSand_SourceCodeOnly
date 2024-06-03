#include "InteractionImplementations/GlyphsWrittingInteraction.h"
#include "Submanagers/GlyphsManager.h"
#include "Managers/MyPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Managers/GameHUD.h"
#include "Widgets/HieroglyphsInputWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGlyphsWrittingInteraction::ImplementInteraction(ALuredToAdventureCharacter *Subject, AActor *Owner)
{
    if (!GlyphsManager) return;
        GlyphsManager->OnSolved.BindUObject(this, &UGlyphsWrittingInteraction::RemoveGlyphsInputWidget);

    if (AMyPlayerController* PC = Subject->GetMyPlayerController())
    {
        FInputModeUIOnly InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->SetShowMouseCursor(true);
        PC->AddGlyphsInputWidget();

        if (AGameHUD* GameHUD = PC->GetHUD<AGameHUD>())
        {
            if (GlyphsWidget = GameHUD->GetGlyphsWidget())
            {
                GlyphsWidget->Stop_btn->OnClicked.AddUniqueDynamic(this, &UGlyphsWrittingInteraction::RemoveGlyphsInputWidget);
                GlyphsManager->ListenGlyphsInputWidget(GlyphsWidget);
            }
        }

    }

    MC = Subject;
    Subject->SetCanInteract(false);
    Subject->ServerRPC_PlayMontage(Subject->CheckingMontage);
	FRotator LookAtCheckable = UKismetMathLibrary::
	FindLookAtRotation(Subject->GetActorLocation(),Owner->GetActorLocation());
	Subject->SlerpToCheckableObject(Subject->GetActorQuat(), FQuat::MakeFromRotator(LookAtCheckable));

}


void UGlyphsWrittingInteraction::ImplementEndInteraction(ALuredToAdventureCharacter *Subject, AActor *Owner)
{
    if (AMyPlayerController* PC = Subject->GetMyPlayerController())
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->SetShowMouseCursor(false);
        
    }
    Subject->SetCanInteract(true);
    Subject->ServerRPC_PlayMontage(Subject->CheckingMontage, FName("ReachOutEnd"));
    if (GlyphsManager) 
    {
        GlyphsManager->RemoveGlyphsInputWidget();
        GlyphsManager->OnSolved.Unbind();
    }
    
}

void UGlyphsWrittingInteraction::BeginPlay()
{
    Super::BeginPlay();

    if (!GlyphsManager)
    {
        AActor* GlyphsActor = UGameplayStatics::GetActorOfClass(this,AGlyphsManager::StaticClass());
        GlyphsManager = CastChecked<AGlyphsManager>(GlyphsActor);
    }
}

void UGlyphsWrittingInteraction::LookAtObject(ALuredToAdventureCharacter *Subject)
{
    Super::LookAtObject(Subject);
}

void UGlyphsWrittingInteraction::RemoveGlyphsInputWidget()
{
    if (!MC) return;
    if (GlyphsWidget)
    {
        GlyphsWidget->Stop_btn->OnClicked.RemoveAll(this);
        GlyphsWidget=nullptr;
    }
    
    MC->EndInteraction();
    MC = nullptr;
    
}
