#include "Managers/MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"    
#include "Interfaces/OnlinePresenceInterface.h"
#include "Online/OnlineSessionNames.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
    bCreateServerAfterDestroy = false;
    DestroyServerName = "";
    ServerNameToFind = "";
    MySessionName = FName("CoopAdventure SessionName");
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
    {
        OnInformationDelegate.Broadcast(FText::FromString(OnlineSubsystem->GetSubsystemName().ToString()));
        if (OnlineSubsystem->GetSubsystemName() == FName("NULL"))
            OnInformationDelegate.Broadcast(FText::FromString("CAUTION: You are not connected to Steam. \n"
            "Both players need to be connected to the Steam platform in order to create and join a server."));

        SessionInterface = OnlineSubsystem->GetSessionInterface();
        if (!SessionInterface.IsValid()) return;
        SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, 
        &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);

        SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, 
        &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
        SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, 
        &UMultiplayerSessionsSubsystem::OnFindSessionComplete);
        SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, 
        &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
    }
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{

}

void UMultiplayerSessionsSubsystem::PrintString(const FString &message)
{
    if (!GEngine) return;
    GEngine->AddOnScreenDebugMessage(-1, 30.f,FColor::Red, message);
}

void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
    OnInformationDelegate.Broadcast(FText::FromString(ServerName));
    if (ServerName.IsEmpty())
    {
        OnInformationDelegate.Broadcast(FText::FromString("Empty server name not allowed."));
        OnServerCreateDelegate.Broadcast(false);
        return;
    }

    if (FNamedOnlineSession* ExistingSessionName = SessionInterface->GetNamedSession(MySessionName))
    {
        OnInformationDelegate.Broadcast(FText::FromString(FString::Printf(TEXT("Session with name %s already exists!"), *MySessionName.ToString())));
        bCreateServerAfterDestroy = true;
        DestroyServerName = MySessionName.ToString();
        SessionInterface->DestroySession(MySessionName);
        return;
    }
    FOnlineSessionSettings SessionSettings;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bIsDedicated = false;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bAllowJoinViaPresence = true;
    SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName()== FName("NULL");
    SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    if (SessionSettings.bIsLANMatch)
        OnInformationDelegate.Broadcast(FText::FromString("Couldn't get Steam subsystem, resorted to LanMatch"));
    SessionInterface->CreateSession(0, MySessionName, SessionSettings);
    return;
}

void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
    OnInformationDelegate.Broadcast(FText::FromString(ServerName));
    if (ServerName.IsEmpty())
    {
        OnClientJoinDelegate.Broadcast(false);
        OnInformationDelegate.Broadcast(FText::FromString("Empty server name not allowed."));
        return;
    }

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName()== FName("NULL");
    SessionSearch->MaxSearchResults = 9999;
    
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    ServerNameToFind = ServerName;
    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

    return;
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        OnInformationDelegate.Broadcast(FText::FromString("Session created successfully!"));
    }
    else
    {
        OnInformationDelegate.Broadcast(FText::FromString("Couldn't create session :/"));
    }
    OnServerCreateDelegate.Broadcast(bWasSuccessful);
    if (bWasSuccessful)
    {
        if (MapPath.IsEmpty()) return;
        FString Path = FString::Printf(TEXT("%s?listen"),*MapPath);
        GetWorld()->ServerTravel(Path);
    }
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    OnInformationDelegate.Broadcast(FText::FromString("OnDestroy session called."));
    if (!bWasSuccessful) return;
    if (!bCreateServerAfterDestroy) return;
    bCreateServerAfterDestroy = false;
    CreateServer(DestroyServerName);
}

void UMultiplayerSessionsSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
    if (!bWasSuccessful || ServerNameToFind.IsEmpty())
    {
        OnInformationDelegate.Broadcast(FText::FromString("Session not found :/"));
        return;
    }

    TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
    FOnlineSessionSearchResult* MatchingResult = 0;
    if (Results.Num()<1)
    {
        OnInformationDelegate.Broadcast(FText::FromString("0 sessions found."));
        OnClientJoinDelegate.Broadcast(false);
        return;
    }
    OnInformationDelegate.Broadcast(FText::FromString(FString::Printf(TEXT("Session Found! Amount: %d"), Results.Num())));
   
    

    for (FOnlineSessionSearchResult result : Results)
    {
        if (!result.IsValid()) continue;
        FString ServerName = "No-name";
        result.Session.SessionSettings.Get(FName("SERVER_NAME"),ServerName);
        
        if (!ServerName.Equals(ServerNameToFind)) continue;
        MatchingResult = &result;
        OnInformationDelegate.Broadcast(FText::FromString(FString::Printf(TEXT("Found Server with name: %s"),*ServerName)));
        break;
    }

    if (MatchingResult)
    {
        SessionInterface->JoinSession(0, MySessionName, *MatchingResult);
    }
    else
    {
        OnInformationDelegate.Broadcast(FText::FromString(FString::Printf(TEXT("Couldn't find server with name: %s"),*ServerNameToFind)));
        ServerNameToFind = "";
        OnClientJoinDelegate.Broadcast(false);
    }
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    OnClientJoinDelegate.Broadcast(Result == EOnJoinSessionCompleteResult::Success);
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        OnInformationDelegate.Broadcast(FText::FromString(FString::Printf(TEXT("Successfully joined session %s"),*SessionName.ToString())));

        FString Address = "";
        bool bWasSuccessful = SessionInterface->GetResolvedConnectString(MySessionName, Address);
        if (bWasSuccessful)
        {
            OnInformationDelegate.Broadcast(FText::FromString(FString::Printf(TEXT("Adress: %s"),*Address)));
            APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
            if (PlayerController)
            {
                OnInformationDelegate.Broadcast(FText::FromString("Travelling to the map"));
                PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
            }
            else
            {
                OnInformationDelegate.Broadcast(FText::FromString("Unable to get player controller"));
            }
        }
        else
        {
            OnInformationDelegate.Broadcast(FText::FromString("GetResolvedConnectString returned false"));
        }
    }
    else
    {
        OnInformationDelegate.Broadcast(FText::FromString("Couldn't join session"));
    }
}
