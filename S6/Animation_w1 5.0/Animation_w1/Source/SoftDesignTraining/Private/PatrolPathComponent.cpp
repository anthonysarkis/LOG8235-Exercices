// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolPathComponent.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "PatrolPathManager.h"
#include "WayPointComponent.h"

// Sets default values for this component's properties
UPatrolPathComponent::UPatrolPathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPatrolPathComponent::BeginPlay()
{
	Super::BeginPlay();

	PatrolPathManager* ppm = PatrolPathManager::GetInstance();

	if (ppm)
	{
		ppm->RegisterPath(this);
	}
	
}

void UPatrolPathComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	PatrolPathManager* ppm = PatrolPathManager::GetInstance();

	if (ppm)
	{
		ppm->UnregisterPath(this);
	}
}

// Called every frame
void UPatrolPathComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	static bool showDebugDisplay = true;
	if (!showDebugDisplay)
		return;

	FVector pathLocation = GetOwner()->GetActorLocation();

	if (m_PatrolPoints.Num() > 0)
	{
		FVector lastPosition = m_PatrolPoints[m_PatrolPoints.Num() - 1]->GetActorLocation();

		for (AActor* children : m_PatrolPoints)
		{
			FVector actorLocation = children->GetActorLocation();

			//DrawDebugLine(GetWorld(), actorLocation, lastPosition, FColor::Blue, false);
			//DrawDebugLine(GetWorld(), actorLocation, pathLocation, FColor::Cyan, false);

			lastPosition = actorLocation;
		}
	}
}

unsigned long UPatrolPathComponent::GetNWayPoints()
{
	return m_PatrolPoints.Num();
}

void UPatrolPathComponent::GetNextWayPoint(long& wayPoint, UWayPointComponent*& nextWaypoint, long direction)
{
	long localWayPoint = (long)wayPoint;

	localWayPoint += direction;
	if (localWayPoint >= m_PatrolPoints.Num())
		localWayPoint = 0;
	if (localWayPoint < 0)
		localWayPoint = m_PatrolPoints.Num() - 1;

	wayPoint = (unsigned long)localWayPoint;

    if (AActor* waypointActor = m_PatrolPoints[wayPoint])
    {
        UWayPointComponent* waypointComponent = dynamic_cast<UWayPointComponent*>(waypointActor->GetComponentByClass(UWayPointComponent::StaticClass()));
        if (waypointComponent != nullptr)
        {
            nextWaypoint = waypointComponent;
        }
    }
}
