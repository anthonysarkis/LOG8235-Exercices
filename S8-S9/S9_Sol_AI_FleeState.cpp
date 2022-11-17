// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/FSM/AI_FleeState.h"
#include "AI/FSM/AI_ChaseState.h"
#include "SoftDesignTraining.h"
#include "AIBase.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AI_FleeState::AI_FleeState()
    :m_timeToStayInFlee(0.f)
{
    m_stateType = EStateType::Flee;
}

AI_FleeState::~AI_FleeState()
{
}

void AI_FleeState::OnEnter(EStateType fromStateId, AAIBase* aiOwner)
{
    if (aiOwner)
    {
        m_timeToStayInFlee = 3.f;
        aiOwner->ProcessScaredDestination(m_fleeDestination, m_timeToStayInFlee);
    }

    //Debug
    {
        FVector npcPosition = aiOwner->GetActorLocation();
        FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
        UWorld* npcWorld = aiOwner->GetWorld();
        DrawDebugSphere(npcWorld, npcHead, 30.0f, 32, FColor::Orange, false, m_timeToStayInFlee);
    }
}

void AI_FleeState::OnCreate()
{

}

void AI_FleeState::OnUpdate(float deltaTime, AAIBase* aiOwner)
{
    if (aiOwner)
    {
        if (m_timeToStayInFlee > 0)
        {
            m_timeToStayInFlee -= deltaTime;
            NavigateToPos(m_fleeDestination, aiOwner);
        }
        else
        {
            m_timeToStayInFlee = 0.f;
            aiOwner->SetAgentIsScared(false);
        }
    }
}

void AI_FleeState::OnExit(EStateType toStateId, AAIBase* aiOwner)
{

}

void AI_FleeState::OnEvaluateTransitions(bool& sucess, EStateType& transitionStateId, AAIBase* aiOwner)
{
    sucess = false;
    //Evaluate in order of priority
    if (ShouldTransitionToChaseState(aiOwner))
    {
        sucess = true;
        transitionStateId = EStateType::Chase;
    }
    else if (ShouldTransitionToPatrolState(aiOwner))
    {
        sucess = true;
        transitionStateId = EStateType::Patrol;
    }
}

bool AI_FleeState::ShouldTransitionToPatrolState(AAIBase* aiOwner)
{
    return !aiOwner->IsAgentScared();
}

bool AI_FleeState::ShouldTransitionToChaseState(AAIBase* aiOwner)
{
    if (!aiOwner->IsAgentScared())
    {
        aiOwner->DetectPlayer();
        return  aiOwner->IsTargetPlayerSeen();
    }
    else
    {
        return false;
    }

}

void AI_FleeState::NavigateToPos(const FVector& pos, AAIBase* aiOwner)
{
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiOwner->GetController(), pos);
}