#include "Exercice3/Animation_Exercice3.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "SDTUtils.h"

#define TURN_DISTANCE 50.0f

void AAnimation_Exercice3::UpdateNavigation_Exercice3(float DeltaTime, float currentSpeed, float targetSpeed, float& outNewSpeed, FVector currentPosition, FVector turnPosition)
{
    // The code & animation speed is defined directly by the speed allowed for the path
    float newSpeed = targetSpeed;
    
    // Handle Turns Here 

    // Set the code & animation speed to the chosen speed
    outNewSpeed = newSpeed;
    AnimationNavigationSpeed = newSpeed;
}

// Call This when your turn is considered completed 
void AAnimation_Exercice3::OnTurnCompleted()
{
    ProceedToNextDesination();
}

