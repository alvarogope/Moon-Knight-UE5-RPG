#include "MKEnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MKPlayerCharacter.h"

const FName AMKEnemyAIController::TargetActorKey(TEXT("TargetActor"));
const FName AMKEnemyAIController::InvestigateLocationKey(TEXT("InvestigateLocation"));

AMKEnemyAIController::AMKEnemyAIController()
{
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

//===================================================//
//----------------- SIGHT DETECTION -----------------//
//===================================================//
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = MoonKnightConstants::EnemySightRadius;
    SightConfig->LoseSightRadius = MoonKnightConstants::EnemySightRadius * 1.25f;
    SightConfig->PeripheralVisionAngleDegrees = MoonKnightConstants::EnemySightAngleDegrees;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

//===================================================//
//----------------- HEAR DETECTION ------------------//
//===================================================//
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = MoonKnightConstants::EnemyHearingRadius;
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

    PerceptionComp->ConfigureSense(*SightConfig);
    PerceptionComp->ConfigureSense(*HearingConfig);
    PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

    PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(
        this, &AMKEnemyAIController::OnTargetPerceptionUpdated);
}

void AMKEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeAsset)
    {
        RunBehaviorTree(BehaviorTreeAsset);
    }
}

void AMKEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    UBlackboardComponent* BB = GetBlackboardComponent();
    if (!BB)
    {
        return;
    }

    const bool bIsPlayer = Cast<AMKPlayerCharacter>(Actor) != nullptr;

    //--- Every pawn is neutral, so ignore other pawns in sight (their noises still get investigated) ---//
    if (!bIsPlayer && Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
    {
        return;
    }

    if (Stimulus.WasSuccessfullySensed())
    {
        if (bIsPlayer)
        {
            BB->SetValueAsObject(TargetActorKey, Actor);
        }
        BB->SetValueAsVector(InvestigateLocationKey, Stimulus.StimulusLocation);
    }
    else if (bIsPlayer)
    {
        //--- LOST SIGHT: drop the target, investigate the last known position ---//
        BB->ClearValue(TargetActorKey);
        BB->SetValueAsVector(InvestigateLocationKey, Stimulus.StimulusLocation);
    }
}