//===================================================//
//-------- AI contoller for humanoid enemies --------//
//===================================================//

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MKPlayerCharacter.h"
#include "Perception/AIPerceptionTypes.h"
#include "MoonKnightRPG.h"
#include "MKEnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UBehaviorTree;

UCLASS()
class MOONKNIGHTRPG_API AMKEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMKEnemyAIController();

protected:
    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION()
    void OnTargetPerceptionUpdated(AACtor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

    static const FName TargetActorKey;
    static const FName InvestigateLocationKey;
}