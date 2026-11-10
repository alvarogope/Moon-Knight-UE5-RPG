#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoonKnightRPG.h"                    
#include "MKPlayerCharacter.generated.h" 

UCLASS()
class MOONKNIGHTRPG_API AMKPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMKPlayerCharacter();

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void LightAttack();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Dodge();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartParry();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ResetCombatState();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EquipWeapon(EWeaponType Weapon);

    //--- NO HEALING IN COMBAT ---//
    UFUNCTION(BlueprintCallable, Category = "Health")
    bool TryHealth(float Amount);

    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsInCombat() const { return bInCombat; }

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealthPercent() const { return MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f; }

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "Death")
    void OnDeathFadeStart(float FadeDuration);

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnComboAttack(int32 ComboIndex);

    void HandleDeath();
    void Respawn();
    void EndStagger();

    //--- Combat flag: cleared after OutOfCombatDelay without attacking or taking damage ---//
    void EnterCombat();
    void LeaveCombat();

    //--- STATE ---//
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = MoonKnightConstants::PlayerMaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth = MoonKnightConstants::PlayerMaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    ECombatState CombatState = ECombatState::Idle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    EWeaponType EquippedWeapon = EWeaponType::Sword;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    int32 ComboCounter = 0;

    UPROPERTY(EditAnywhere, Category = "Combat")
    int32 MaxComboLength = MoonKnightConstants::MaxComboHits;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bInCombat = false;

    //--- Fallback respawn point when no willow tree has been registered ---//
    FVector SpawnLocation = FVector::ZeroVector;

    FTimerHandle RespawnTimerHandle;
    FTimerHandle CombatStateTimerHandle;
    FTimerHandle LeaveCombatTimerHandle;
};