// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTNavigation/SoftDesignAIAvoidance.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "PatrolPathComponent.h"
#include "ReactionManager.h"
#include "PatrolPathManager.h"

// helper functions

static float VectorSign(const FVector& Vec, const FVector& A, const FVector& B)
{
    return FMath::Sign((B.X - A.X) * (Vec.Y - A.Y) - (B.Y - A.Y) * (Vec.X - A.X));
}

static bool IsPointInCone(const FVector& TestPoint, const FVector& A, const FVector& B, const FVector& C, const FVector& D)
{
    float BA = VectorSign(B, A, TestPoint);
    float CB = VectorSign(C, B, TestPoint);
    float DC = VectorSign(D, C, TestPoint);
    float AD = VectorSign(A, D, TestPoint);

    // point is in the same direction of all 4 cone edge lines
    // must be inside, regardless of cone winding
    return BA == CB && CB == DC && DC == AD;
}

void ASoftDesignAIAvoidance::BeginPlay()
{
    AAIBase::BeginPlay();

    GetCharacterMovement()->MaxWalkSpeed = (float)(rand() % 100 + 35);
}

// Called every frame
void ASoftDesignAIAvoidance::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + GetVelocity(), 5.0f, FColor::Yellow, false, -1.0f, 0.0, 2.0f);
}

void ASoftDesignAIAvoidance::ProcessAvoidance(float DeltaTime)
{
//   DisplayDebugAvoidance();
        
    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        // fetch all the registered NPCs
        int npcCount = reactionManager->m_NPCList.Num();

        for (int i = 0; i < npcCount; ++i)
        {
            AActor* npcCharacter = reactionManager->m_NPCList[i];
            if (npcCharacter && npcCharacter != this) // we found an agetn different than current agent
            {
                // first exercise: 
                // Enable ProcessSimpleAvoidance and observe the behaviour of the agents
                // Some code was stripped out of this function and avoidance is broken
                // it is your task to rewrite some code to make sure the agents avoid each other
                // the point is to write something simple to make the agent deviate from its trajectory
                //ProcessSimpleAvoidance(npcCharacter, DeltaTime);

                // second exercise : 
                // Enable ProcessVOAvoidance and observe the behaviour of the agents.
                // play with some parameters inside this function to see the impact of each one of them.
                //ProcessVOAvoidance(npcCharacter, DeltaTime);
            }
        }
    }
}

// This function is meant to produce a very simple code for collision avoidance. 
// The idea is: when I detect a coming collision, I drastically change my velocity to change direction and avoid collision
void ASoftDesignAIAvoidance::ProcessSimpleAvoidance(AActor* npcObstacle, float DeltaTime)
{
    if (!npcObstacle)
        return;

    // use the radius of agent and obstacle to detect collision
    float agentRadius = GetSimpleCollisionRadius();
    float obstacleRadius = npcObstacle->GetSimpleCollisionRadius();
    float totalAvoidanceRadius = (agentRadius + obstacleRadius);

    // if agent and obstacle getting dangerously closer, do something
    if (GetHorizontalDistanceTo(npcObstacle) < (totalAvoidanceRadius) * 3.0f) // you can play with the coeff value here to observe change in behavior
    {
        // some useful vectors...
        FVector velocity = GetVelocity();
        FVector baseI(1.0f, 0.0f, 0.0f);
        FVector baseJ(0.0f, 1.0f, 0.0f);
        FVector baseK(0.0f, 0.0f, 1.0f);

        // compute force here

        // update the agent's velocity with your force
        ApplyNewVelocity(/* put new velocity here */ FVector(), DeltaTime);
    }
}

// This function is meant to produce a more complicated code for collision avoidance.
// this is a simpler version of what can be used nowadays in video games but the idea stays the same.
// it uses a velocity obstacle to detect collision and update velocity.
// This code is still quite a simple version, we work with 2 agents, so there is only one VO per agent. Things get more complicated with many more agents.
void ASoftDesignAIAvoidance::ProcessVOAvoidance(AActor* npcObstacle, float DeltaTime)
{
    if (!npcObstacle)
        return;

    float agentRadius = GetSimpleCollisionRadius();
    float obstacleRadius = npcObstacle->GetSimpleCollisionRadius();
    float totalAvoidanceRadius = (agentRadius + obstacleRadius); // 4. add a coefficient of *1.5f to give the agents a bigger vital space

    // build cone Orca-like
    float windowOfTime = 10.0f;
    FVector obstacleCircleCenter = (npcObstacle->GetActorLocation() - GetActorLocation());
    FVector agentCircleCenter = obstacleCircleCenter / windowOfTime;
    float agentCircleRadius = totalAvoidanceRadius / windowOfTime;

    // build the cone using the tangents of the circles.
    // to get the tangents, use perpendicular vectors to the segment (agent circle center -> obstacle circle center)
    FVector vectorBetweenCircles = obstacleCircleCenter - agentCircleCenter;
    FVector baseK(0.0f, 0.0f, 1.0f);
    FVector perpVect = FVector::CrossProduct(vectorBetweenCircles, baseK).GetSafeNormal2D();
    FVector obstaclePerpVect = perpVect * totalAvoidanceRadius;
    FVector agentPerpVect = perpVect * agentCircleRadius;
    FVector sideConeObs1 = obstacleCircleCenter + obstaclePerpVect;
    FVector sideConeObs2 = obstacleCircleCenter - obstaclePerpVect;
    FVector sideConeAgent1 = agentCircleCenter + agentPerpVect;
    FVector sideConeAgent2 = agentCircleCenter - agentPerpVect;

    // 1. enable this part of code to see the cone and understand the behaviour as it goes.
    /* 
    DrawDebugLine(GetWorld(), GetActorLocation() + sideConeObs1, GetActorLocation() + sideConeAgent1, FColor::Blue, false, -1.0f, 0.0, 3.0f);
    DrawDebugLine(GetWorld(), GetActorLocation() + sideConeObs2, GetActorLocation() + sideConeAgent2, FColor::Blue, false, -1.0f, 0.0, 3.0f);
    DrawDebugLine(GetWorld(), GetActorLocation() + sideConeAgent1, GetActorLocation() + sideConeAgent2, FColor::Blue, false, -1.0f, 0.0, 3.0f);
    */

    // project the lines to create a bigger cone.
    FVector closingConePt1 = sideConeAgent1 + (sideConeObs1 - sideConeAgent1)*5;
    FVector closingConePt2 = sideConeAgent2 + (sideConeObs2 - sideConeAgent2)*5;

    // if velocity is inside the cone, it means there is a collision detected.
    FVector newVelocity = GetVelocity();
    if (IsPointInCone(GetVelocity(), closingConePt1, sideConeAgent1, sideConeAgent2, closingConePt2))
    {        
        // find closest point on cone line 1 and cone line 2
        FVector closest1 = FMath::ClosestPointOnSegment(GetVelocity(), sideConeAgent1, closingConePt1);
        FVector closest2 = FMath::ClosestPointOnSegment(GetVelocity(), sideConeAgent2, closingConePt2);
        
        // 2. enable this part of code to see the see the two possible choices we have to leave the cone.
        /*
        DrawDebugPoint(GetWorld(), GetActorLocation() + closest1, 20.0f, FColor::Green);
        DrawDebugPoint(GetWorld(), GetActorLocation() + closest2, 20.0f, FColor::Red);
        */

        float distanceTo1 = (closest1 - GetVelocity()).Size2D();
        float distanceTo2 = (closest2 - GetVelocity()).Size2D();

        // 3. Enable this part of code to see avoidance performed
        /*
        bool needSpeedCorrection = false;
        if (distanceTo1 < distanceTo2) // chose closest point outside the cone => modify least current velocity 
        {
            newVelocity = closest1;
            needSpeedCorrection = closest1.Equals(sideConeAgent1);
        }
        else
        {
            newVelocity = closest2;
            needSpeedCorrection = closest2.Equals(sideConeAgent2);
        }

        if (needSpeedCorrection)
        {
            newVelocity = newVelocity.GetUnsafeNormal() * GetVelocity().Size();
        }
        */
    }

    ApplyNewVelocity(newVelocity, DeltaTime);
}

void ASoftDesignAIAvoidance::ApplyNewVelocity(const FVector& newVelocity, float DeltaTime)
{
    UPawnMovementComponent* movementComponent = GetMovementComponent();
    if (movementComponent)
    {
        const FQuat Rotation = movementComponent->UpdatedComponent->GetComponentQuat();
        const FVector OldLocation = movementComponent->UpdatedComponent->GetComponentLocation();

        FHitResult Hit(1.f);

        movementComponent->SafeMoveUpdatedComponent(newVelocity * DeltaTime, Rotation, true, Hit);

        const FVector NewLocation = movementComponent->UpdatedComponent->GetComponentLocation();
        movementComponent->Velocity = ((NewLocation - OldLocation) / DeltaTime);

        movementComponent->UpdateComponentVelocity();
    }
}

void ASoftDesignAIAvoidance::DisplayDebugAvoidance() const 
{
    FVector npcPosition = GetActorLocation();
    FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
    UWorld* npcWorld = GetWorld();
    float npcRadius = GetSimpleCollisionRadius();

    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        // fetch all the registered NPCs
        int npcCount = reactionManager->m_NPCList.Num();

        for (int i = 0; i < npcCount; ++i)
        {
            AActor* npcCharacter = reactionManager->m_NPCList[i];
            if (npcCharacter && npcCharacter != this) // we found an agent different than current agent
            {
                FVector obstaclePosition = npcCharacter->GetActorLocation();
                float obstacleRadius = npcCharacter->GetSimpleCollisionRadius();

                // draw spheres according to their collision status
                if (GetHorizontalDistanceTo(npcCharacter) < (npcRadius + obstacleRadius))
                {
                    // agents colliding or way too close to each other
                    DrawDebugSphere(npcWorld, npcHead, 20.0f, 32, FColor::Red);
                }
                else if (GetHorizontalDistanceTo(npcCharacter) < (npcRadius + obstacleRadius) * 3.0f)
                {
                    // agents getting closer to each other, possible collision ahead
                    DrawDebugSphere(npcWorld, npcHead, 20.0f, 32, FColor::Orange);
                }
                else if (GetHorizontalDistanceTo(npcCharacter) < (npcRadius + obstacleRadius) * 6.0f)
                {
                    // agents close to each other without danger of collision
                    DrawDebugSphere(npcWorld, npcHead, 20.0f, 32, FColor::Yellow);
                }
            }

        }
    }
}
