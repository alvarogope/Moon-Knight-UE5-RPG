#include "MKGameInstance.h"
#include "MKPlayerCharacter.h"

void UMKGameInstance::RegisterWillowTree(const FVector& TreeLocation)
{
    LastWillowTreeLocation = TreeLocation;
    bHasCheckpoint = true;
}

void UMKGameInstance::RespawnAtLastWillowTree(AMKPlayerCharacter* Player)
{
    if (!Player)
    {
        return;
    }

//------------- WILLOW TREE DEATH RESPAWN -------------//
    if (bHasCheckpoint)
    {
        Player->SetActorLocation(LastWillowTreeLocation, false, nullptrd,
            ETeleportType::TeleportPhysics);
    }
}