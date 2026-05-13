// APlayerCharacter.h
// Moon Knight — Player Character Base Class
// Registers core components and exposes properties used by Blueprint subclass.
// Combat logic, animation, and input handling are implemented in Blueprints.
// This class establishes the C++ foundation those Blueprints build on.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UQuantumAbilityComponent.h"
#include "APlayerCharacter.generated.h"

// Combat state — drives animation Blueprint transitions and input gating
UENUM(BlueprintType)
enum class ECombatState : uint8
{
    Idle            UMETA(DisplayName = "Idle"),
    Attacking       UMETA(DisplayName = "Attacking"),   // IsAttacking = true
    Dodging         UMETA(DisplayName = "Dodging"),     // Mid-combo dodge available at any point
    HitReacting     UMETA(DisplayName = "Hit Reacting"),
    Dead            UMETA(DisplayName = "Dead")
};

UCLASS()
class MOONKNIGHT_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // -------------------------------------------------------
    // STATS
    // Health drives the moon-phase HUD bar via SetPercent
    // -------------------------------------------------------

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
    float MaxHealth = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
    float CurrentHealth = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Combat")
    float BaseDamage = 200.f;  // Applied via ApplyDamage on sword trace hit

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Progression")
    int32 Level = 1;

    // -------------------------------------------------------
    // COMBAT STATE
    // Managed by BPC_Attack System Blueprint component
    // -------------------------------------------------------

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    ECombatState CombatState = ECombatState::Idle;

    // Attack index drives the Switch on Int combo chain (0 = reset, 1-4 = attacks)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 AttackIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bIsAttacking = false;

    // SaveAttack: true when player inputs during an active attack, queues next in chain
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bSaveAttack = false;

    // -------------------------------------------------------
    // TARGET LOCK
    // Sphere trace (r=200, range=1500) from camera forward
    // Filters PhysicsBody + Pawn, validated by "Damageable" tag
    // -------------------------------------------------------

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|TargetLock")
    AActor* ActorToTargetLock = nullptr;

    // -------------------------------------------------------
    // QUANTUM ABILITY COMPONENT
    // Manages The Power of the Gods skill tree state
    // -------------------------------------------------------

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
    UQuantumAbilityComponent* QuantumAbilityComponent;
};
