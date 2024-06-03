#pragma once

UENUM(BlueprintType)
enum class ECrocoStates : uint8
{
    EEE_Idle UMETA(DisplayName = "Idle"),
    EEE_Patrol UMETA(DisplayName = "Patrol"),
    EEE_Chase UMETA(DisplayName = "Chase"),
    EEE_UpBite UMETA(DisplayName = "Up Bite"),
    EEE_ForwardBite UMETA(DisplayName = "Forward Bite"),
    EEE_ReturnToOrigins UMETA(DisplayName = "Return")
};