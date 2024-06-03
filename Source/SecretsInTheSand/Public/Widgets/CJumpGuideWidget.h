#pragma once

#include "CoreMinimal.h"
#include "Widgets/StartingGuideWidget.h"
#include "CJumpGuideWidget.generated.h"

UCLASS()
class SECRETSINTHESAND_API UCJumpGuideWidget : public UStartingGuideWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media", meta = (AllowPrivateAccess="true"))
	TObjectPtr<class UMediaPlayer> MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media", meta = (AllowPrivateAccess="true"))
	TObjectPtr<class UMediaPlayer> MediaPlayer2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media", meta = (AllowPrivateAccess="true"))
	TObjectPtr<class UMediaSource> KneelExample;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media", meta = (AllowPrivateAccess="true"))
	TObjectPtr<class UMediaSource> ImpulseExample;
	
};
