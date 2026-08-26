//===================================================//
//------------------- GAME MODULE -------------------//
//===================================================//

#pragma once

#include "CoreMinimal.h"
#include "MoonKnightRPG.generated.h"
#include <cstdint>


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

ENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Sword   UMETA(DisplayName = "Sword"),
    Bow     UMETA(DisplayName = "Bow")
};


namespace MoonKnightConstants 
{
    //--- Death and Respawn ---
    constexpr float DeathFadeDuration      = 5.0f;
    constexpr float RespawnDelay           = 4.9f;

    //--- Enemy Perception (sound and sight) ---
    constexpr float EnemySightRadius       = 1200.0f;
    constexpr float EnemySightAngleDegrees = 90.0f;
    constexpr float EnemyHearingRadius     = 800.0f;
    
    //--- Skill tree caps ---
    constexpr float MaxBowDamageBonus      = 100.0f;
}