// MoonKnightRPG.h
// Moon Knight — Solo-developed third-person action RPG in Unreal Engine 5
// All values extracted directly from Blueprint implementation.
// Gameplay logic implemented in Unreal Engine 5 Blueprints.

#pragma once

#include "CoreMinimal.h"

// ============================================================
// COMBAT SYSTEM
// 4-hit light attack combo chain with a combo finisher.
// Implemented via a Switch on Int driven by AttackIndex (0-4).
// Player can dodge or be hit mid-combo at any point.
// ============================================================

UENUM(BlueprintType)
enum class EAttackType : uint8
{
    None            UMETA(DisplayName = "None"),           // AttackIndex = 0 (reset state)
    LightAttack1    UMETA(DisplayName = "Light Attack 1"), // AttackIndex = 1
    LightAttack2    UMETA(DisplayName = "Light Attack 2"), // AttackIndex = 2
    LightAttack3    UMETA(DisplayName = "Light Attack 3"), // AttackIndex = 3
    LightAttack4    UMETA(DisplayName = "Light Attack 4"), // AttackIndex = 4 — triggers finisher
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
    Idle            UMETA(DisplayName = "Idle"),
    Attacking       UMETA(DisplayName = "Attacking"),  // IsAttacking = true
    Dodging         UMETA(DisplayName = "Dodging"),    // Interruptible mid-combo
    HitReacting     UMETA(DisplayName = "Hit Reacting"),
    Dead            UMETA(DisplayName = "Dead")
};

// Sword trace runs on a looping timer (0.001s interval) between
// SwordTopPoint and SwordBottomPoint bones during active attacks.
// Hit detection uses Actor Tag "Damageable" to filter valid targets.
USTRUCT(BlueprintType)
struct FSwordTraceConfig
{
    GENERATED_BODY()

    // Sphere trace radius — player weapon
    UPROPERTY(BlueprintReadWrite, Category = "Combat|Trace")
    float PlayerTraceRadius = 120.f;

    // Sphere trace radius — enemy weapon (wider for AI hit detection)
    UPROPERTY(BlueprintReadWrite, Category = "Combat|Trace")
    float EnemyTraceRadius = 300.f;

    // Looping timer interval for continuous hit detection during attack
    UPROPERTY(BlueprintReadWrite, Category = "Combat|Trace")
    float TraceInterval = 0.001f;

    // Debug draw time in editor
    UPROPERTY(BlueprintReadWrite, Category = "Combat|Trace")
    float DrawTime = 0.3f;
};

// ============================================================
// PLAYER STATS
// Health is the primary active stat. Stamina bar exists in HUD
// but is not consumed by attacks in the current build.
// Level is displayed in HUD via Z_To_Text(Float).
// ============================================================

USTRUCT(BlueprintType)
struct FPlayerStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Stats|Health")
    float MaxHealth = 100.f;

    UPROPERTY(BlueprintReadWrite, Category = "Stats|Health")
    float CurrentHealth = 100.f;

    // Base damage applied via ApplyDamage on sword trace hit
    UPROPERTY(BlueprintReadWrite, Category = "Stats|Combat")
    float BaseDamage = 200.f;

    UPROPERTY(BlueprintReadWrite, Category = "Stats|Progression")
    int32 Level = 1;
};

// ============================================================
// ENEMY AI — STANDARD
// Behaviour Tree: BD_AI
// States: Patrol → Investigate (on sound) → Chase → Attack
// GetDistanceToTarget service ticks every 0.40-0.60s
// ============================================================

USTRUCT(BlueprintType)
struct FStandardEnemyConfig
{
    GENERATED_BODY()

    // Distance at which the enemy transitions from chase to attack
    UPROPERTY(BlueprintReadWrite, Category = "AI|Combat")
    float MeleeAttackDistance = 250.f;

    // MoveTo stop distance when chasing player
    UPROPERTY(BlueprintReadWrite, Category = "AI|Movement")
    float ChaseStopDistance = 100.f;

    // Random patrol point radius
    UPROPERTY(BlueprintReadWrite, Category = "AI|Patrol")
    float PatrolRadius = 3000.f;

    // MoveTo stop distance when patrolling
    UPROPERTY(BlueprintReadWrite, Category = "AI|Patrol")
    float PatrolStopDistance = 120.f;

    // Wait time at each patrol point
    UPROPERTY(BlueprintReadWrite, Category = "AI|Patrol")
    float PatrolWaitTime = 2.f;

    // How long the enemy waits at the investigation location before resuming patrol
    UPROPERTY(BlueprintReadWrite, Category = "AI|Investigation")
    float InvestigationWaitTime = 9.f;

    // Base damage per hit applied via ApplyDamage
    UPROPERTY(BlueprintReadWrite, Category = "AI|Combat")
    float BaseDamage = 140.f;

    // Sword trace radius (wider than player for AI reliability)
    UPROPERTY(BlueprintReadWrite, Category = "AI|Combat")
    float SwordTraceRadius = 300.f;
};

// ============================================================
// ENEMY AI — MINI BOSS (WEREWOLF)
// Behaviour Tree: BD_Werewolf
// Simpler BT than standard: Patrol ↔ Combat only (no investigation).
// Blackboard key: seeingTarget? (Is Set / Is Not Set).
// Uses BT_Task_BossChaseTarget and BT_Task_BossAttacks via
// Simple Parallel — chases and attacks simultaneously.
// Has a dedicated health bar widget (WB_Werewolf_H).
// ============================================================

USTRUCT(BlueprintType)
struct FMiniBossConfig
{
    GENERATED_BODY()

    // Boss health — drives WB_Werewolf_H health bar via SetPercent(Health / MaxHealth)
    UPROPERTY(BlueprintReadWrite, Category = "MiniBoss|Stats")
    float MaxHealth = 350.f;

    UPROPERTY(BlueprintReadWrite, Category = "MiniBoss|Stats")
    float CurrentHealth = 350.f;

    // Patrol radius — same as standard enemy
    UPROPERTY(BlueprintReadWrite, Category = "MiniBoss|Patrol")
    float PatrolRadius = 3000.f;

    UPROPERTY(BlueprintReadWrite, Category = "MiniBoss|Patrol")
    float PatrolStopDistance = 120.f;

    UPROPERTY(BlueprintReadWrite, Category = "MiniBoss|Patrol")
    float PatrolWaitTime = 2.f;
};

// ============================================================
// INVENTORY & EQUIP SYSTEM
// Slot-based inventory backed by a Data Table (S_Slots / S_Items).
// Items store: Name, Damage, Icon, StaticMesh, Type, WeaponSocket.
// Equipping swaps the weapon StaticMesh on the character socket.
// Supports Sword and Bow slots.
// ============================================================

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Weapon          UMETA(DisplayName = "Weapon"),
    Armour          UMETA(DisplayName = "Armour"),
    Consumable      UMETA(DisplayName = "Consumable"),
    QuestItem       UMETA(DisplayName = "Quest Item")
};

USTRUCT(BlueprintType)
struct FSItems  // Matches S_Items Data Table row struct
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    FName Name;

    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    float Damage = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    UTexture2D* Icon = nullptr;

    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    UStaticMesh* StaticMesh = nullptr;

    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    EItemType Type = EItemType::Weapon;

    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    FName WeaponSocket;
};

// ============================================================
// PICKUP SYSTEM
// Sphere trace fired from camera forward vector on IA_Interact.
// Casts hit actor to BP_Weapon and calls AddWeapon on success.
// The weapon actor is destroyed after pickup.
// ============================================================

USTRUCT(BlueprintType)
struct FPickupTraceConfig
{
    GENERATED_BODY()

    // Sphere trace radius for weapon pickup detection
    UPROPERTY(BlueprintReadWrite, Category = "Pickup")
    float TraceRadius = 7.f;

    // Trace length from camera forward (Get Forward Vector * Range)
    UPROPERTY(BlueprintReadWrite, Category = "Pickup")
    float TraceRange = 1000.f;
};

// ============================================================
// TARGET LOCK SYSTEM
// Sphere trace from camera forward on IA_TargetLock input.
// Filters by PhysicsBody and Pawn object types.
// Validates target using Actor Has Tag "Damageable".
// Toggle: if Actor to Target Lock is valid, clears it (unlock).
// ============================================================

USTRUCT(BlueprintType)
struct FTargetLockConfig
{
    GENERATED_BODY()

    // Sphere trace radius for target detection
    UPROPERTY(BlueprintReadWrite, Category = "TargetLock")
    float TraceRadius = 200.f;

    // Trace length from camera forward
    UPROPERTY(BlueprintReadWrite, Category = "TargetLock")
    float TraceRange = 1500.f;
};

// ============================================================
// DEATH / RESPAWN
// On Die event: disables input, enables mesh physics simulation,
// fades camera (0→1 over 5s), waits 4.9s, reloads current level.
// ============================================================

USTRUCT(BlueprintType)
struct FDeathConfig
{
    GENERATED_BODY()

    // Camera fade duration on death
    UPROPERTY(BlueprintReadWrite, Category = "Death")
    float CameraFadeDuration = 5.f;

    // Delay before level reload
    UPROPERTY(BlueprintReadWrite, Category = "Death")
    float RespawnDelay = 4.9f;
};

// ============================================================
// GAME CONSTANTS
// ============================================================

namespace MoonKnightConstants
{
    // Combat
    constexpr int32 MAX_COMBO_HITS          = 4;       // Attacks before combo finisher
    constexpr float SWORD_TRACE_INTERVAL    = 0.001f;  // Looping hit detection timer (s)
    constexpr float PLAYER_BASE_DAMAGE      = 200.f;
    constexpr float ENEMY_BASE_DAMAGE       = 140.f;
    constexpr float PLAYER_TRACE_RADIUS     = 120.f;
    constexpr float ENEMY_TRACE_RADIUS      = 300.f;

    // AI
    constexpr float MELEE_ATTACK_DISTANCE   = 250.f;
    constexpr float CHASE_STOP_DISTANCE     = 100.f;
    constexpr float PATROL_RADIUS           = 3000.f;
    constexpr float PATROL_STOP_DISTANCE    = 120.f;
    constexpr float PATROL_WAIT_TIME        = 2.f;
    constexpr float INVESTIGATION_WAIT      = 9.f;
    constexpr float AI_SERVICE_TICK_MIN     = 0.4f;
    constexpr float AI_SERVICE_TICK_MAX     = 0.6f;

    // Mini-Boss
    constexpr float BOSS_MAX_HEALTH         = 350.f;

    // Pickup & Interaction
    constexpr float PICKUP_TRACE_RADIUS     = 7.f;
    constexpr float PICKUP_TRACE_RANGE      = 1000.f;

    // Target Lock
    constexpr float TARGET_LOCK_RADIUS      = 200.f;
    constexpr float TARGET_LOCK_RANGE       = 1500.f;

    // Death
    constexpr float CAMERA_FADE_DURATION    = 5.f;
    constexpr float RESPAWN_DELAY           = 4.9f;
}
