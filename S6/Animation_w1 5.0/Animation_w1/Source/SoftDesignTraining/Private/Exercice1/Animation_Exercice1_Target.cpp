#include "Exercice1/Animation_Exercice1_Target.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "SDTUtils.h"

#define WALK_ANIMATION_SPEED 200.0f
#define RUN_ANIMATION_SPEED 500.0f

void AAnimation_Exercice1_Target::UpdateNavigation_Exercice1(float DeltaTime, float targetSpeed)
{
    AnimationNavigationSpeed = targetSpeed;
}
