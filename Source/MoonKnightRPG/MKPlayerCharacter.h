#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoonKnightRPG.h"                    
#include "MKPlayerCharacter.generated.h" 
#include "MoonKnightRPH.h"

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

    //--- NO HEALING IN COMBAT ---//
    UFUNCTION(BlueprintCallable, Category = "Health")
    bool tryHealth(float Amount);

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

    //--- STATE ---//
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    ECombatState CombatState = ECombatState::Idle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")ç
    EWeaponType EquippedWeapon = EWeaponType::Sword;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 ComboCounter = 0;

    UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxComboLength = 3;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bInCombat = false;

    FTimerHandle RespawnTimerHandle;
    FTimerHandle ComboResetTimerHandle;
};