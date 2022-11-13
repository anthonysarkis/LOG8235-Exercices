#include "Animation/AIPatrol.h"
#include "SoftDesignTraining.h"
#include "PatrolPathComponent.h"
#include "PatrolPathManager.h"
#include "DrawDebugHelpers.h"

#pragma optimize ("", off)

AAIPatrol::AAIPatrol()
{
    m_LinearSpeedChangeTime = 0.0f;
    m_LinearSpeedChangeTimer = 0.0f;
    m_InitialSpeed = 0.0f;
    m_TargetSpeed = 0.0f;
    m_LastFrameSpeed = 0.0f;
    m_WaitTime = 0.0f;
    PatrolPathActor = nullptr;
}

void AAIPatrol::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAIPatrol::ProcessMovement(float DeltaTime)
{
    if (GetNoExercice() != 3)
    {
        if (HasReachedDestination(GetActorLocation()))
        {
            UpdateReachedDestination();
        }
    }


    UpdateNavigation(DeltaTime);

    //DrawDebugLine(GetWorld(), GetActorLocation(), m_CurrentDestination, FColor::Green, false);
}

void AAIPatrol::UpdateNavigation(float DeltaTime)
{
    float desiredSpeed = m_TargetSpeed;
    if (m_WaitTime > 0.0f)
    {
        m_WaitTime -= DeltaTime;
        desiredSpeed = 0.0f;
    }

    if (GetNoExercice() == 1)
    {
        UpdateNavigation_Exercice1(DeltaTime, desiredSpeed);
        GetCharacterMovement()->MaxWalkSpeed = desiredSpeed;
    }
    else if (GetNoExercice() == 2)
    {
        float newSpeed = desiredSpeed;
        UpdateNavigation_Exercice2(DeltaTime, m_LastFrameSpeed, desiredSpeed, newSpeed);
        GetCharacterMovement()->MaxWalkSpeed = newSpeed;
        m_LastFrameSpeed = newSpeed;

        UpdateLayeredAction_Exercice2(m_LayeredAction);
    }
    else if (GetNoExercice() == 3)
    {
        AActor* waypointActor = GetWayPointAtIndex(m_CurrentWayPoint);

        FVector currentPosition = GetActorLocation();
        FVector waypointPosition = m_CurrentDestination;
        float newSpeed = desiredSpeed;
        UpdateNavigation_Exercice3(DeltaTime, m_LastFrameSpeed, desiredSpeed, newSpeed, currentPosition, waypointPosition);

        GetCharacterMovement()->MaxWalkSpeed = newSpeed;
        m_LastFrameSpeed = newSpeed;
    }
    else
    {
        UpdateCurrentSpeed(DeltaTime);
    }
}

void AAIPatrol::SetInitialPatrolling()
{
    if (PatrolPathActor != nullptr)
    {
        UPatrolPathComponent* patrolPathComponent = dynamic_cast<UPatrolPathComponent*>(PatrolPathActor->GetComponentByClass(UPatrolPathComponent::StaticClass()));
        if (patrolPathComponent != nullptr)
        {
            m_PatrolPath = patrolPathComponent;
            m_PathDirection = 1;
            m_CurrentWayPoint = 0;
            if (AActor* waypointActor = GetWayPointAtIndex(m_CurrentWayPoint))
            {
                SetDestinationData(*waypointActor);
                SetupSpeedTransition();
            }
        }
    }
}

void AAIPatrol::SetMaxWalkingSpeed()
{
    GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

void AAIPatrol::DetectPlayer()
{
}

void AAIPatrol::UpdateCurrentSpeed(float DeltaTime)
{
    GetCharacterMovement()->MaxWalkSpeed = m_TargetSpeed;
}

void AAIPatrol::UpdateNextDestination()
{
    if (m_CurrentWayPoint >= 0 && m_CurrentWayPoint < m_PatrolPath->m_PatrolPoints.Num())
    {
        int32 nextIndex = m_CurrentWayPoint + m_PathDirection;
        if (AActor* waypointActor = GetWayPointAtIndex(nextIndex))
        {
            m_CurrentWayPoint = nextIndex;
            SetDestinationData(*waypointActor);
        }
    }
}

void AAIPatrol::SetDestinationData(const AActor& waypointActor)
{
    m_CurrentDestination = waypointActor.GetActorLocation();
    SetNewMoveDestination(m_CurrentDestination);

    UWayPointComponent* waypointComponent = dynamic_cast<UWayPointComponent*>(waypointActor.GetComponentByClass(UWayPointComponent::StaticClass()));
    if (waypointComponent != nullptr)
    {
        m_NextWaypoint = waypointComponent;
    }
}

AActor* AAIPatrol::GetWayPointAtIndex(int32 index)
{
    if (m_PatrolPath)
    {
        if (AActor* patrolPathOwner = m_PatrolPath->GetOwner())
        {
            TArray<AActor*>& waypoints = m_PatrolPath->m_PatrolPoints;
            if (index < waypoints.Num())
            {
                return waypoints[index];
            }
        }
    }

    return nullptr;
}

void AAIPatrol::SetupSpeedTransition()
{
    if (m_NextWaypoint != nullptr)
    {
        m_LinearSpeedChangeTime = m_NextWaypoint->TimeToReachSpeed;
        m_TargetSpeed = m_NextWaypoint->TargetSpeed;
        m_InitialSpeed = GetCharacterMovement()->MaxWalkSpeed;
        m_LinearSpeedChangeTimer = m_LinearSpeedChangeTime;
        m_WaitTime = m_NextWaypoint->WaitTime;
        m_LayeredAction = m_NextWaypoint->LayeredAction;

        if (m_LinearSpeedChangeTime == 0.0f)
        {
            GetCharacterMovement()->MaxWalkSpeed = m_NextWaypoint->TargetSpeed;
        }
    }
}

bool AAIPatrol::HasReachedDestination(const FVector actorLocation)
{
    return (actorLocation - m_CurrentDestination).Size2D() < 50.f;
}

void AAIPatrol::UpdateReachedDestination()
{
    ProceedToNextDesination();
}

void AAIPatrol::ProceedToNextDesination()
{
    if (AActor* waypointActor = GetWayPointAtIndex(m_CurrentWayPoint))
    {
        UWayPointComponent* waypointComponent = dynamic_cast<UWayPointComponent*>(waypointActor->GetComponentByClass(UWayPointComponent::StaticClass()));
        if (waypointComponent != nullptr)
        {
            if (waypointComponent->IsADirectionChange &&
                m_CurrentWayPoint + m_PathDirection > m_PatrolPath->m_PatrolPoints.Num() - 1 ||
                m_CurrentWayPoint + m_PathDirection < 0)
            {
                m_PathDirection = m_PathDirection *= -1.0f;
                UpdateNextDestination();
                SetupSpeedTransition();
            }
            else
            {
                UpdateNextDestination();
                SetupSpeedTransition();
            }

            m_StopTimer = waypointComponent->TimeToStop;
        }
    }
}