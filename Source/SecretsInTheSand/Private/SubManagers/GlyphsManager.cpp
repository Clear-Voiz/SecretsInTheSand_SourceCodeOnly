#include "SubManagers/GlyphsManager.h"
#include "InteractableActors/CheckableActor.h"
#include "Managers/MyPlayerController.h"
#include "Widgets/HieroglyphsInputWidget.h"
#include "Managers/GameHUD.h"
#include "InteractionImplementations/GlyphsWrittingInteraction.h"
#include "Net/UnrealNetwork.h"
#include "Components/DecalComponent.h"
#include "Components/Button.h"

AGlyphsManager::AGlyphsManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

}

void AGlyphsManager::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;
	AssignInscriptionsToTables();


}

void AGlyphsManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGlyphsManager, bAlreadyActivated);
}

void AGlyphsManager::AssignInscriptionsToTables()
{
	FString FinalPassword = "";
	for (int32 i=0; i<InscriptionOptions.Num()/GlyphsPerRow; i++)
	{
		int32 RandomIndex = FMath::RandRange(i*5, ((i+1)*GlyphsPerRow)); //
		RandomIndex = FMath::Clamp<int32>(RandomIndex,0,((i+1)*GlyphsPerRow)-1);
		FString TargetGlyph = InscriptionOptions[RandomIndex];
		FinalPassword.Append(TargetGlyph);
		FText Result = FText::FromString(TargetGlyph);
		KeyGlyphs.Add(Result);
		InscriptionTables[i]->Glyph = Result;
	}

	Password = FText::FromString(FinalPassword);
}

void AGlyphsManager::ListenGlyphsInputWidget(UHieroglyphsInputWidget* GlyphsWidget)
{
	GlyphsWidget->OnCheckInputGlyphs.BindUObject(this, &AGlyphsManager::SendServerCheckGlyphs);

}

void AGlyphsManager::RemoveGlyphsInputWidget()
{
	if (AMyPlayerController* PC = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
	{
		AGameHUD* HUD = PC->GetHUD<AGameHUD>();
		if (!HUD) return;
		if (UHieroglyphsInputWidget* GlyphsWidget = HUD->GetGlyphsWidget())
		{
			GlyphsWidget->OnCheckInputGlyphs.Unbind();
		}
		PC->RemoveGlyphsInputWidget();
	}

}

void AGlyphsManager::SendServerCheckGlyphs(const FText& ReceivedCombination)
{
	if (bAlreadyActivated) return;
	if (AMyPlayerController* PC = GetWorld()->GetFirstPlayerController<AMyPlayerController>())
		PC->ServerRPC_CheckGlyphs(this, ReceivedCombination);
}

void AGlyphsManager::CheckGlyphs(const FText& ReceivedCombination)
{
	if (Password.EqualTo(ReceivedCombination))
	{
		OnActivated.Broadcast();
		bAlreadyActivated = true;
		OnRep_ActivateMechanisms();
	}

}

void AGlyphsManager::OnRep_ActivateMechanisms() 
{
	OnSolved.ExecuteIfBound();
	if (InscriptionCanvases.IsEmpty()) return;
	for (TObjectPtr<ACheckableActor> InscriptionCanvas : InscriptionCanvases)
	{
		UDecalComponent* DecalGlyphs = Cast<UDecalComponent>(InscriptionCanvas->FindComponentByClass(UDecalComponent::StaticClass()));
		if (!DecalGlyphs) continue;
		DecalGlyphs->SetHiddenInGame(false);
	}
}

