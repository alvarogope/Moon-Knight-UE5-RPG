#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MKPlayerCharacter.h"
#include "MoonKnightRPG.h"
#include "MKGameInstance.generated.h"

class AMKPlayerCharacter;

UCLASS()
class MOONKNIGHTRPG_API UMKGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
//------------- WILLOW TREE CHECKPOINTS -------------//
    UFUNCTION(BlueprintCallable, Category = "Checkpoint")
    void RegisterWillowTree(const FVector& TreeLocation);

    UFUNCTION(BlueprintCallable, Category = "Checkpoint")
    void RespawnAtLastWillowTree(AMKPlayerCharacter* Player);

    UFUNCTION(BlueprintPure, Category = "Checkpoint")
    bool HasCheckpoint() const { return bHasCheckpoint; }

//--------------- EQUIPMENT PERSISTENCE ---------------//
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void SaveEquippedWeapon(EWeaponType Weapon) { SavedWeapon = Weapon; }

    UFUNCTION(BlueprintPure, Category = "Equipment")
    EWeaponType GetSavedWeapon() const { return SavedWeapon; }

protected:
    UPROPERTY(VisibleAnywhere, Category = "Checkpoint")
    FVector LastWillowTreeLocation = FVector::ZeroVector;

    UPROPERTY(VisibleAnywhere, Category = "Checkpoint")
    bool bHasCheckpoint = false;

    UPROPERTY(VisibleAnywhere, Category = "Equipment")
    EWeaponType SavedWeapon = EWeaponType::Sword;
};