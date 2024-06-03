#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerCreateDelegate, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClientJoinDelegate, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInformationDelegate, FText, Message);


UCLASS()
class SECRETSINTHESAND_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UMultiplayerSessionsSubsystem();

	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

	void PrintString(const FString& message);

	IOnlineSessionPtr SessionInterface;

	bool bCreateServerAfterDestroy;

	FString DestroyServerName;
	FString ServerNameToFind;
	FName MySessionName;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UFUNCTION(BlueprintCallable)
	void CreateServer(FString ServerName);

	UFUNCTION(BlueprintCallable)
	void FindServer(FString ServerName);

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION()
	void OnFindSessionComplete(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UPROPERTY(BlueprintAssignable)
	FServerCreateDelegate OnServerCreateDelegate;

	UPROPERTY(BlueprintAssignable)
	FClientJoinDelegate OnClientJoinDelegate;

	UPROPERTY(BlueprintAssignable)
	FInformationDelegate OnInformationDelegate;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	FString MapPath;
	
};
