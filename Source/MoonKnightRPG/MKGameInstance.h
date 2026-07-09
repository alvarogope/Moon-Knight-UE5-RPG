// UMoonKnightGameInstance.h
// Moon Knight — Game Instance (Equip State Persistence)
//
// PROBLEM THIS SOLVES:
// Equipment state (currently equipped sword/bow) was being stored in the
// player character actor. When a new level loaded, the actor was destroyed
// and recreated — wiping all equipped item data. Players lost their
// equipment on every level transition.
//
// SOLUTION:
// Equipment state is now owned by the Game Instance, which persists across
// level loads for the entire game session. On level load, the new player
// character reads its initial equipment state from here rather than
// starting from defaults.
//
// This is a deliberate architectural decision: data that must survive level
// transitions belongs in Game Instance, not in per-level actors.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UMoonKnightGameInstance.generated.h"

// Matches the S_Slots and S_Items Data Table row structs used in Blueprints.
// Weapon rows store: Name, Damage, Icon, StaticMesh, Type, WeaponSocket.

UENUM(BlueprintType)
enum class EEquippedWeaponSlot : uint8
{
    None    UMETA(DisplayName = "None"),
    Sword   UMETA(DisplayName = "Sword"),
    Bow     UMETA(DisplayName = "Bow")
};

USTRUCT(BlueprintType)
struct FEquippedItemState
{
    GENERATED_BODY()

    // Row name from the weapon Data Table
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FName ItemRowName = NAME_None;

    // Which slot this item occupies
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    EEquippedWeaponSlot Slot = EEquippedWeaponSlot::None;

    // Whether this slot currently has an item equipped
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    bool bIsEquipped = false;
};

UCLASS()
class MOONKNIGHT_API UMoonKnightGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UMoonKnightGameInstance();

    // -------------------------------------------------------
    // EQUIPMENT STATE
    // Persists across level loads for the full game session.
    // Read by player character on BeginPlay to restore loadout.
    // Written by BPC_Equipment System Blueprint on equip/unequip.
    // -------------------------------------------------------

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FEquippedItemState EquippedSword;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FEquippedItemState EquippedBow;

    // Called by equipment Blueprint to persist state before level transition
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void SaveEquipmentState(FEquippedItemState Sword, FEquippedItemState Bow);

    // Called by player character BeginPlay to restore loadout after level load
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void LoadEquipmentState(FEquippedItemState& OutSword, FEquippedItemState& OutBow);
};
