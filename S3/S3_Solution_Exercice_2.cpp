// Fill out your copyright notice in the Description page of Project Settings.

#include "Exo2/SDTAIControllerExercice2.h"
#include "SoftDesignTraining.h"
#include "Exo2/SDTProjectile.h"
#include "DrawDebugHelpers.h"
#include "Exo2/PawnDataContainerComponent.h"
#include "Components/SplineComponent.h"
#include "PhysicsHelpers.h"

bool ASDTAIControllerExercice2::MoveToTarget(FVector2D target, float speed, float deltaTime)
{
	APawn* pawn = GetPawn();
	FVector const pawnPosition(pawn->GetActorLocation());
	FVector2D const toTarget = target - FVector2D(pawnPosition);
	FVector2D const displacement = FMath::Min(toTarget.Size(), speed * deltaTime) * toTarget.GetSafeNormal();
	pawn->SetActorLocation(pawnPosition + FVector(displacement, 0.f), true);
	pawn->SetActorRotation(FVector(displacement, 0.f).ToOrientationQuat());
	return toTarget.Size() < speed * deltaTime;
}

void ASDTAIControllerExercice2::MoveTowardsDirection(FVector2D direction, float speed, float deltaTime)
{
	APawn* pawn = GetPawn();
	FVector const pawnPosition(pawn->GetActorLocation());
	FVector2D const displacement = speed * deltaTime * direction.GetSafeNormal();
	pawn->SetActorLocation(pawnPosition + FVector(displacement, 0.f), true);
	pawn->SetActorRotation(FVector(direction, 0.f).ToOrientationQuat());
}

bool ASDTAIControllerExercice2::ReachSpline(USplineComponent const& spline, float deltaTime)
{
	FVector2D const actorPosition(GetPawn()->GetActorLocation());
	FVector2D const splineStartPoint = FindSplineClosestPosition(spline);
	return MoveToTarget(splineStartPoint, m_maxSpeed, deltaTime);
}

bool ASDTAIControllerExercice2::FollowSpline(USplineComponent const& spline, float deltaTime)
{
	FVector const actorPosition(GetPawn()->GetActorLocation());
	FVector const direction = spline.FindDirectionClosestToWorldLocation(actorPosition, ESplineCoordinateSpace::World);
	MoveTowardsDirection(FVector2D(direction), m_maxSpeed, deltaTime);

	return (spline.FindInputKeyClosestToWorldLocation(GetPawn()->GetActorLocation()) / spline.GetNumberOfSplinePoints() - 1) > 0.99f;
}

bool ASDTAIControllerExercice2::GetAroundObstacle(float deltaTime)
{
	if (m_hitInformation.HandleHit())
	{
		FVector2D const contactDirection = FVector2D(FVector::CrossProduct(FVector::UpVector, FVector(m_hitInformation.m_hitNormal, 0.0f)));
		MoveTowardsDirection(contactDirection, m_maxSpeed, deltaTime);
		return false; // Not done going around the obstacle
	}

	return true; // No more obstacle
}

float ASDTAIControllerExercice2::GetDistanceToSpline(USplineComponent const& spline) const
{
	FVector2D const actorPosition(GetPawn()->GetActorLocation());
	FVector2D const splineStartPoint = FindSplineClosestPosition(spline);
	return (splineStartPoint - actorPosition).Size();
}

bool ASDTAIControllerExercice2::FollowSplineInDynamicObstacles(USplineComponent const& spline, float deltaTime)
{
	float newSpeed = m_maxSpeed;
	FVector2D newDirection = FVector2D(spline.FindDirectionClosestToWorldLocation(GetPawn()->GetActorLocation(), ESplineCoordinateSpace::World));

	TArray<CollisionInfo> collisions = RecoverIncomingCollisions(newDirection.GetSafeNormal() * newSpeed);
	for (CollisionInfo const& collision : collisions)
	{
		if (collision.m_timeToCollision < 0.5f)
		{
			newSpeed = 0.f;
		}
	}

	MoveTowardsDirection(newDirection, newSpeed, deltaTime);
	return (spline.FindInputKeyClosestToWorldLocation(GetPawn()->GetActorLocation()) / (spline.GetNumberOfSplinePoints() - 1)) > 0.99f;
}

USplineComponent* ASDTAIControllerExercice2::GetClosestSplineWithoutObstacle(TArray<USplineComponent*>& splines) const
{
	Algo::Sort(splines, [&](USplineComponent* lhs, USplineComponent* rhs)
		{
			return GetDistanceToSpline(*lhs) < GetDistanceToSpline(*rhs);
		});

	for (USplineComponent* spline : splines)
	{
		FVector2D const splineStartPoint = FindSplineClosestPosition(*spline);
		FVector2D const pawnPosition(GetPawn()->GetActorLocation());

		UWorld* World = GetWorld();
		PhysicsHelpers physicHelper(World);

		TArray<struct FHitResult> hitResult;
		physicHelper.CastRay(FVector(pawnPosition, 100.f), FVector(splineStartPoint, 100.f), hitResult, true);

		if (hitResult.Num() == 0)
		{
			return spline;
		}
	}

	return nullptr;
}

FVector2D ASDTAIControllerExercice2::FindSplineClosestPosition(USplineComponent const& spline) const
{
	FVector2D const actorPosition(GetPawn()->GetActorLocation());
	return FVector2D(spline.FindLocationClosestToWorldLocation(FVector(actorPosition, 0.f), ESplineCoordinateSpace::World));
}

TArray<FOverlapResult> ASDTAIControllerExercice2::CollectTargetActorsInFrontOfCharacter(APawn const* pawn, PhysicsHelpers& physicHelper) const
{
	TArray<FOverlapResult> outResults;
	physicHelper.SphereOverlap(pawn->GetActorLocation() + pawn->GetActorForwardVector() * 500.0f, 500.0f, outResults, true);
	return outResults;
}