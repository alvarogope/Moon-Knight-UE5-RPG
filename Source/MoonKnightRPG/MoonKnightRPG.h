// Primary game module header for Moon-Knight
// Owns the shared types and design constants used across the module, so that
//    tuning values from the Game Design Document live in exactly one place.

#pragma once

#include "CoreMinimal.h"
#include "MoonKnightRPG.generated.h"

/*
    Combat states for the Moon-Knight and humanoid enemies.
    Single definition for the whole module ---> DO NOT REDELCARE IN OTHER HEADERS
    The player character, enemy AI, and animation Blueprints all read this.
*/

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

/*
    Weapon categories available to the Moon-knight (GDD Ch. 5: sword, bow;
    alchemy/magic reserved for the specialisation path).
 */

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