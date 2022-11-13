#include "Exercice4/Animation_Exercice4.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "SDTUtils.h"

/*
param in
    npcPosition : The position of the npc doing the look at
    targetPosition : The position of the target the npc is looking at
    headSpaceWorld : The head transform of the npc
    deltaTime : The delta time for the frame
param out
    outHeadTransform : The pointer of the transform to modify with the result of the lookat. It will be used to update the skeleton.
*/

void AAnimation_Exercice4::ProcessLookAt_Exercice4(FVector npcPosition, FVector targetPosition, FTransform headTransformWorldSpace, float deltaTime, FTransform& outHeadTransform)
{

}