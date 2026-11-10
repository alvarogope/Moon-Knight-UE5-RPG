//===================================================//
//------------------- GAME MODULE -------------------//
//===================================================//

#pragma once

#include "CoreMinimal.h"
#include <cstdint>
#include "MoonKnightRPG.generated.h"


UENUM(BlueprintType)
enum class ECombatState : uint8
{
    Idle            UMETA(DisplayName = "Idle"),
    Attacking       UMETA(DisplayName = "Attacking"),  
    Dodging         UMETA(DisplayName = "Dodging"),    
    Parrying	    UMETA(DisplayName = "Parrying"),
	Staggered	    UMETA(DisplayName = "Staggered"),
    Dead            UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Sword   UMETA(DisplayName = "Sword"),
    Bow     UMETA(DisplayName = "Bow")
};


namespace MoonKnightConstants 
{
    //--- Player ---
    constexpr float PlayerMaxHealth          = 100.0f;
    constexpr int32 MaxComboHits             = 4;

    //--- Death and Respawn ---
    constexpr float DeathFadeDuration        = 5.0f;
    constexpr float RespawnDelay             = 4.9f;

    //--- Enemy Perception (sound and sight) ---
    constexpr float EnemySightRadius         = 1200.0f;
    constexpr float EnemyLoseSightMultiplier = 1.25f;
    constexpr float EnemySightAngleDegrees   = 90.0f;
    constexpr float EnemySightMaxAge         = 5.0f;
    constexpr float EnemyHearingRadius       = 800.0f;
    constexpr float EnemyHearingMaxAge       = 3.0f;
    
    //--- Skill tree caps ---
    constexpr float MaxBowDamageBonus        = 100.0f;
    
    //--- Combat ---
    constexpr float ComboResetDelay          = 1.2f;
    constexpr float DodgeDuration            = 0.6f;
    constexpr float ParryWindow              = 0.4f;
    constexpr float StaggerDuration          = 0.5f;
    constexpr float OutOfCombatDelay         = 5.0f;
}
