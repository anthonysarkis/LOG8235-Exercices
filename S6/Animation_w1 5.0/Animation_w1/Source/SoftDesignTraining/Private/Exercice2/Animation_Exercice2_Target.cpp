#include "Exercice2/Animation_Exercice2_Target.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "SDTUtils.h"

// UpdateNavigation_Exercice2
//
// deltaTime : The delta time for the frame
// currentSpeed : The current speed for the actor
// targetSpeed : The target speed for the actor
// outNewSpeed : (out) The resulting speed computed by the UpdateNavigation_Exercice2 function, which will be used for the movement

void AAnimation_Exercice2_Target::UpdateNavigation_Exercice2(float DeltaTime, float currentSpeed, float targetSpeed, float& outNewSpeed)
{
    float newSpeed = SDTUtils::ExponentialDamp(currentSpeed, targetSpeed, 0.5f, DeltaTime);

    AnimationNavigationSpeed = newSpeed; // Set the target speed for the animation
    outNewSpeed = newSpeed; // Set the target speed for the controller (out)
}

// UpdateLayeredAction_Exercice2
//
// layeredAction : The layered action to play on the path

void AAnimation_Exercice2_Target::UpdateLayeredAction_Exercice2(UPatrolLayeredAction layeredAction)
{
    IsWaving = layeredAction == UPatrolLayeredAction::Waving;
}