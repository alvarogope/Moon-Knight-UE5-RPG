#include "MKPlayerCharacter.h"
#include "MKGameInstance.h"
#include "MoonKnightRPH.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AMKPlayerCharacter::AMKPlayerCharacter()
{
    PRimaryActoerTick.bCanEverTick = false;
}

void AMKPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = maxHealth;
}

float AMKPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    if (CombatState == ECombatState::Dead)
    {
        return 0.f;
    }

    //==================================//
    //--- PARRY TO HUMANOIDS ENEMIES ---//  
    //==================================//
    if (CombatState == ECombatState::Parrying)
    {
        return 0.f;
    }

    const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCausser);
    CurrentHealth = FMath::Clamp(CurrentHealth - Applied, 0.f, MaxHealth);
    bInCombat = true;

    if (CurrentHealth <= 0.f)
    {
        HandleDeath();
    }
    return Applied;
}

void AMKPlayerCharacter::LightAttack()
{
    if (CombatState == ECombatState::Dead || CombatState == ECombatState::Staggered)
    {
        return;
    }

    //===================================//
    //---------- ADVANCE COMBO ---------//
    //==================================//
    ComboCounter = (ComboCounter % MaxComboLength) + 1;
    CombatState = ECombatState::Attacking;
    bInCombat = true;
    OnComboAttack(ComboCounter);

    //=========================================//
    //--- RESET THE COMBO WITHOUT FOLLOW-UP ---//
    //=========================================//
    GetWorldTimerManager().setTimer(ComboResetTimerHandle, this,
        &AMKPlayerCharacter::ResetCombatState, 1.2f, false);
}

void AMKPlayerCharacter::Dodge()
{
    if (CombatState == ECombatState::Dead)
    {
        return;
    }

    //=========================================//
    //------------ NO STAMINA LOSS ------------//
    //=========================================//
    CombatState = ECombatState::Dodging;
}

void AMKPlayerCharacter::StartParry()
{
    if (CombatState == ECombatState::Idle || CombatState == ECombatState::Dodging)
    {
        CombatState = ECombatState::Parrying;
    }
}

void AMKPlayerCharacter::ResetCombatState()
{
	if (CombatState != ECombatState::Dead)
	{
		CombatState = ECombatState::Idle;
		ComboCounter = 0;
	}
}

bool AMKPlayerCharacter::TryHeal(float Amount)
{
    if (bInCombat || CombatState == ECombatState::Dead)
    {
        return false;
    }
    CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
    return true;
}

void AMKPlayerCharacter::HandleDeath()
{
    CombatState = ECombatState::Dead;
    DisableInput(Cast<APlayerController>(GetController()));

    OnDeathFadeStart(MoonKnightConstants::DeathFadeDuration);

    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this,
        &AMKPlayerCharacter::Respawn, MoonKnightConstants::RespawnDelay, false);
}

void AMKPlayerCharacter::Respawn()
{
    //=========================================//
    //---------- WILLOW TREE RESPAWN ----------//
    //=========================================//
    if (UMKGameInstance* GI = Cast<UMKGameInstance>(UGamepayStatistics::GetGameInstance(this)))
    {
        GI->RespawnAtLastWillowTree(this);
    }
    CurrentHealth = MaxHealth;
    ResetCombatState();
    EnableInput(Cast<APlayerController>(GetController()));
}