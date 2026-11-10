#include "MKPlayerCharacter.h"
#include "MKGameInstance.h"
#include "MoonKnightRPG.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AMKPlayerCharacter::AMKPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMKPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    SpawnLocation = GetActorLocation();
    CurrentHealth = MaxHealth;

    //--- RESTORE THE WEAPON SAVED IN THE GAME INSTANCE ---//
    if (UMKGameInstance* GI = Cast<UMKGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        EquippedWeapon = GI->GetSavedWeapon();
    }
}


float AMKPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    if (CombatState == ECombatState::Dead)
    {
        return 0.f;
    }

    //======================================//
    //--- PARRY NEGATES ANY INCOMING HIT ---//
    //======================================//
    if (CombatState == ECombatState::Parrying)
    {
        return 0.f;
    }

    //====================================//
    //--- DODGE INVULNERABILITY FRAMES ---//
    //====================================//
    if (CombatState == ECombatState::Dodging)
    {
        return 0.f;
    }

    const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (Applied <= 0.f)
    {
        return 0.f;
    }

    CurrentHealth = FMath::Clamp(CurrentHealth - Applied, 0.f, MaxHealth);
    EnterCombat();

    if (CurrentHealth <= 0.f)
    {
        HandleDeath();
        return Applied;
    }

    //============================================//
    //--- STAGGER: INPUT LOCKED UNTIL RECOVERY ---//
    //============================================//
    CombatState = ECombatState::Staggered;
    ComboCounter = 0;
    DisableInput(Cast<APlayerController>(GetController()));
    GetWorldTimerManager().SetTimer(CombatStateTimerHandle, this,
        &AMKPlayerCharacter::EndStagger, MoonKnightConstants::StaggerDuration, false);

    return Applied;
}

void AMKPlayerCharacter::EndStagger()
{
    if (CombatState != ECombatState::Staggered)
    {
        return;
    }

    EnableInput(Cast<APlayerController>(GetController()));
    ResetCombatState();
}

void AMKPlayerCharacter::LightAttack()
{
    if (CombatState != ECombatState::Idle && CombatState != ECombatState::Attacking)
    {
        return;
    }

    if (ComboCounter >= MaxComboLength)
    {
        return;
    }

    ++ComboCounter;
    CombatState = ECombatState::Attacking;
    EnterCombat();
    OnComboAttack(ComboCounter);

    GetWorldTimerManager().SetTimer(CombatStateTimerHandle, this,
        &AMKPlayerCharacter::ResetCombatState, MoonKnightConstants::ComboResetDelay, false);
}

void AMKPlayerCharacter::Dodge()
{
    //--- Only from Idle or mid-combo: no dodge spam, no dodge out of stagger or parry ---//
    if (CombatState != ECombatState::Idle && CombatState != ECombatState::Attacking)
    {
        return;
    }

    //=========================================//
    //------------ NO STAMINA LOSS ------------//
    //=========================================//
    CombatState = ECombatState::Dodging;

    //--- Shares the combo timer handle, so a pending combo reset can't cut the dodge short ---//
    GetWorldTimerManager().SetTimer(CombatStateTimerHandle, this,
        &AMKPlayerCharacter::ResetCombatState, MoonKnightConstants::DodgeDuration, false);
}

void AMKPlayerCharacter::StartParry()
{
    if (CombatState == ECombatState::Idle)
    {
        CombatState = ECombatState::Parrying;

        GetWorldTimerManager().SetTimer(CombatStateTimerHandle, this,
            &AMKPlayerCharacter::ResetCombatState, MoonKnightConstants::ParryWindow, false);
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

void AMKPlayerCharacter::EquipWeapon(EWeaponType Weapon)
{
    EquippedWeapon = Weapon;

    if (UMKGameInstance* GI = Cast<UMKGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        GI->SaveEquippedWeapon(Weapon);
    }
}

bool AMKPlayerCharacter::TryHealth(float Amount)
{
    if (bInCombat || CombatState == ECombatState::Dead)
    {
        return false;
    }
    CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
    return true;
}

void AMKPlayerCharacter::EnterCombat()
{
    bInCombat = true;
    GetWorldTimerManager().SetTimer(LeaveCombatTimerHandle, this,
        &AMKPlayerCharacter::LeaveCombat, MoonKnightConstants::OutOfCombatDelay, false);
}

void AMKPlayerCharacter::LeaveCombat()
{
    bInCombat = false;
}

void AMKPlayerCharacter::HandleDeath()
{
    CombatState = ECombatState::Dead;
    GetWorldTimerManager().ClearTimer(CombatStateTimerHandle);
    DisableInput(Cast<APlayerController>(GetController()));

    OnDeathFadeStart(MoonKnightConstants::DeathFadeDuration);

    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this,
        &AMKPlayerCharacter::Respawn, MoonKnightConstants::RespawnDelay, false);
}


//=========================================//
//---------- WILLOW TREE RESPAWN ----------//
//=========================================//
void AMKPlayerCharacter::Respawn()
{
    UMKGameInstance* GI = Cast<UMKGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GI && GI->HasCheckpoint())
    {
        GI->RespawnAtLastWillowTree(this);
    }
    else
    {
        SetActorLocation(SpawnLocation, false, nullptr, ETeleportType::TeleportPhysics);
    }

    //--- Set directly: ResetCombatState() ignores the Dead state ---//
    CurrentHealth = MaxHealth;
    CombatState = ECombatState::Idle;
    ComboCounter = 0;
    GetWorldTimerManager().ClearTimer(LeaveCombatTimerHandle);
    bInCombat = false;
    EnableInput(Cast<APlayerController>(GetController()));
}
