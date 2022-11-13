#include "Exercice3/Animation_Exercice3_Target.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "SDTUtils.h"

#pragma optimize("", off)

#define TURN_DISTANCE 50.0f

void AAnimation_Exercice3_Target::UpdateNavigation_Exercice3(float DeltaTime, float currentSpeed, float targetSpeed, float& outNewSpeed, FVector currentPosition, FVector stopPosition)
{
    float newSpeed = currentSpeed;

    float distanceToStop = (currentPosition - stopPosition).Size2D();
    float timeToReachPoint = distanceToStop / currentSpeed;

    //Slowdown the actor when coming near the waypoint, you can use a time o reach instead of purely a distance to behave better across varied speeds
    if (!ApproachStarted && timeToReachPoint < 0.5f)
    {
        ApproachStarted = true;
    }

    if (!AnimationTurnAction && distanceToStop < TURN_DISTANCE)
    {
        AnimationTurnAction = true;
    }

    // Stop the actor motion while it is turning
    if (AnimationTurnAction)
    {
        newSpeed = 0.0f;
    }
    else if (ApproachStarted)
    {
        float stopApproachSpeed = 50.0f;
        newSpeed = SDTUtils::ExponentialDamp(currentSpeed, stopApproachSpeed, 0.3f, DeltaTime);
    }
    else
    {
        newSpeed = SDTUtils::ExponentialDamp(currentSpeed, targetSpeed, 0.5f, DeltaTime);
    }

    outNewSpeed = newSpeed;
    AnimationNavigationSpeed = newSpeed;
}

void AAnimation_Exercice3_Target::OnTurnCompleted()
{
    ApproachStarted = false;
    AnimationTurnAction = false;
    ProceedToNextDesination();
}
