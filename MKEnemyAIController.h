// AEnemyAIController.h
// Moon Knight — Enemy AI Controller Base Class
// Base for both standard enemy and mini-boss (Werewolf) AI controllers.
// Each enemy type runs its own Behaviour Tree:
//   Standard enemy: BD_AI    (Patrol -> Investigate -> Chase -> Attack)
//   Mini-boss:      BD_Werewolf (Patrol / Combat via Simple Parallel)
//
// Perception is handled by UAIPerceptionComponent using sight stimulus.
// On detection, TargetActor is written to the Blackboard.
// On losing sight, the Blackboard key is cleared.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AEnemyAIController.generated.h"

UCLASS()
class MOONKNIGHT_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

protected:
    virtual void BeginPlay() override;

    // Called by UE5 Perception System when a stimulus is detected or lost
    UFUNCTION()
    virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
    // -------------------------------------------------------
    // PERCEPTION
    // Sight radius varies by enemy type:
    //   Standard enemy: 1200 units, 90 degree FOV
    //   Mini-boss: wider aggro radius
    // Hearing radius: 800 units (triggers investigation)
    // -------------------------------------------------------

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
    UAIPerceptionComponent* PerceptionComponent;

    // -------------------------------------------------------
    // BEHAVIOUR TREE
    // Assigned per enemy subclass:
    //   BP_AIController      -> BD_AI
    //   BP_AIController_MiniBoss -> BD_Werewolf
    // -------------------------------------------------------

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    class UBehaviorTree* BehaviorTree;

    // -------------------------------------------------------
    // BLACKBOARD KEYS
    // TargetActor: set on sight detection, cleared on sight loss
    // InvestigationLocation: set on hearing detection
    // -------------------------------------------------------

    // Blackboard key name for the target actor reference
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Blackboard")
    FName TargetActorKey = FName("Target Actor");

    // Blackboard key name for the investigation location
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Blackboard")
    FName InvestigationLocationKey = FName("Investigation Location");

    // Write target to Blackboard on detection
    void SetTarget(AActor* Target);

    // Clear Blackboard target key on sight loss
    void ClearTarget();
};
