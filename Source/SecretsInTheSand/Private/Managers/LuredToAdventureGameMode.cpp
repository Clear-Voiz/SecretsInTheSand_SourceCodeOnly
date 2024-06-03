// Copyright Epic Games, Inc. All Rights Reserved.

#include "Managers/LuredToAdventureGameMode.h"
#include "Managers/LTA_GameStateBase.h"
#include "Managers/MyPlayerController.h"
#include "Patterns/Command_CollectItem.h"
#include "Patterns/Command_Pedestals.h"
#include "UObject/ConstructorHelpers.h"
#include "Interfaces/Death.h"
#include "Characters/LuredToAdventureCharacter.h"
#include "Net/UnrealNetwork.h"

ALuredToAdventureGameMode::ALuredToAdventureGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

void ALuredToAdventureGameMode::PostLogin(APlayerController *NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(NewPlayer);
	if (!PlayerController) return;

	PlayerControllers.Add(PlayerController);
	PlayerController->OnPlayerControllerDeath.AddUObject(this, &ALuredToAdventureGameMode::Respawn);
	Characters.Add(PlayerController->GetPawn());
}

void ALuredToAdventureGameMode::InitGameState()
{
	Super::InitGameState();
	ALTA_GameStateBase* MyGameState = Cast<ALTA_GameStateBase>(GameState);
	if (MyGameState) MyGameState->OnGameStatePointsCollected.BindUObject(this,&ALuredToAdventureGameMode::AddCommandAndUpdateScore);

}

void ALuredToAdventureGameMode::UpdateScore(const int32& Amount)
{
	ALTA_GameStateBase* MyGameState = Cast<ALTA_GameStateBase>(GameState);
	if (MyGameState) MyGameState->UpdateScore(Amount);
}

void ALuredToAdventureGameMode::AddCommandAndUpdateScore(UClass* ItemType, const int32& Amount, const FVector& Location)
{
	UCommand_CollectItem* NewCommand = NewObject<UCommand_CollectItem>(this);
	NewCommand->Init(ItemType,Amount,Location);
	PendingCommands.Add(NewCommand);
	UpdateScore(Amount);

}

void ALuredToAdventureGameMode::AddCommand(ASecurityAlarm* Alarm)
{
	UCommand_Pedestals* PedestalCommand = NewObject<UCommand_Pedestals>(this);
	PedestalCommand->Init(Alarm);
	PendingCommands.Add(PedestalCommand);
}

void ALuredToAdventureGameMode::ClearCommands()
{
	PendingCommands.Empty();
}

void ALuredToAdventureGameMode::Respawn(FVector RespawnLoc)
{
	for(AMyPlayerController* PlayerController : PlayerControllers)
	{
		FTransform RespawnPoint = FindPlayerStart(PlayerController)->GetActorTransform();
		PlayerController->ClientRPC_Respawn(RespawnPoint);
		if (AMyPlayerController* PC = Cast<AMyPlayerController>(PlayerController))
		{
			PC->EndRumbling();
		}

		APawn* Player = PlayerController->GetPawn();
		if (Player->Implements<UDeath>())
		{	IDeath* DeathInterface = CastChecked<IDeath>(Player);
			Player->SetActorLocation(DeathInterface->RespawnLocation);
		}

	}


	while(!PendingCommands.IsEmpty())
	{
		PendingCommands.Pop()->Undo();
	}

}

void ALuredToAdventureGameMode::SetCharactersVisibility(bool bHide)
{
	for (TObjectPtr<AActor> AdvenChara : Characters)
	{
		if (!AdvenChara) 
		{
			continue;
		}
		AdvenChara->SetActorHiddenInGame(bHide);
	}
}

UClass *ALuredToAdventureGameMode::GetDefaultPawnClassForController_Implementation(AController *InController)
{

	if (CurrentCharToAssign)
	{
		if (!RedCharLook || !BlueCharLook) return DefaultPawnClass;
		if (CurrentCharToAssign == RedCharLook)
		{
			CurrentCharToAssign = BlueCharLook;
		}
		else
		{
			CurrentCharToAssign = RedCharLook;
		}
	}
	else
	{
		if (!RedCharLook || !BlueCharLook) return DefaultPawnClass;
		CurrentCharToAssign = ((rand() % 2) == 0) ? RedCharLook : BlueCharLook;
	}

	return CurrentCharToAssign;
}

void ALuredToAdventureGameMode::UnlockComboJump()
{
	for(AActor* Character : Characters)
	{
		if (ALuredToAdventureCharacter* MC = Cast<ALuredToAdventureCharacter>(Character))
		{
			MC->SetCanComboJump(true);
		}
	}
}

void ALuredToAdventureGameMode::StopSpawnedSounds()
{
	for (AActor* PlayerCharacter : Characters)
	{
		if (ALuredToAdventureCharacter* MC = Cast<ALuredToAdventureCharacter>(PlayerCharacter))
			MC->ClientRPC_StopSpawnedSounds();
	}
}
