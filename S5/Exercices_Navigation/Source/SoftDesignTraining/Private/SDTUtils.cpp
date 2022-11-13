// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTUtils.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "AIBase.h"
#include "NavigationSystem.h"

/*static*/ bool SDTUtils::Raycast(UWorld* uWorld, FVector sourcePoint, FVector targetPoint)
{
    FHitResult hitData;
    FCollisionQueryParams TraceParams(FName(TEXT("VictoreCore Trace")), true);

    return uWorld->LineTraceSingleByChannel(hitData, sourcePoint, targetPoint, ECC_Pawn, TraceParams);
}

/*static*/ bool SDTUtils::RaycastNavMesh(UWorld* uWorld, FVector sourcePoint, FVector targetPoint)
{
    FVector hitLocation;
    UNavigationSystemV1::NavigationRaycast(uWorld, sourcePoint, targetPoint, hitLocation);

    return (hitLocation - targetPoint).IsNearlyZero();
}
