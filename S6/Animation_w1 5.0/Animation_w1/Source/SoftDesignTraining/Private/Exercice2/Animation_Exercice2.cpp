#include "Exercice2/Animation_Exercice2.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"


// currentValue : The current value of the smoothed value
// targetValue : The target value of the smoothed value
// dampingFactor : The factor at which you want the current value to reach the target value
// deltaTime : The delta time for the frame
static float SmoothSpeed(const float& currentValue, const float& targetValue, float dampingFactor, float deltaTime)
{
    return targetValue;
}

// UpdateNavigation_Exercice2
//
// deltaTime : The delta time for the frame
// currentSpeed : The current speed for the actor
// targetSpeed : The target speed for the actor
// outNewSpeed : (out) The resulting speed computed by the UpdateNavigation_Exercice2 function, which will be used for the movement

void AAnimation_Exercice2::UpdateNavigation_Exercice2(float deltaTime, float currentSpeed, float targetSpeed, float& outNewSpeed)
{
    float newSpeed = SmoothSpeed(currentSpeed, targetSpeed, 1.0f, deltaTime);
    
    AnimationNavigationSpeed = newSpeed; // Set the target speed for the animation
    outNewSpeed = newSpeed; // Set the target speed for the controller (out)
}

// UpdateLayeredAction_Exercice2
//
// layeredAction : The layered action to play on the path

void AAnimation_Exercice2::UpdateLayeredAction_Exercice2(UPatrolLayeredAction layeredAction)
{

}