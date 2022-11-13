// LOG8235 - August 2020

#include "Exo1/Exercice1.h"
#include "SoftDesignTraining.h"
#include <cmath>
#include "DrawDebugHelpers.h"
#include "TargetActor.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void AExercice1::GetVisibilityInformationForTarget(ATargetActor* targetActor, FVector selfPosition, PhysicsHelpers& physicHelper, bool& canSee, bool& canShoot)
{
	TArray<struct FHitResult> hitResult;
	physicHelper.CastRay(selfPosition, targetActor->GetActorLocation(), hitResult, true);

	canSee = true;
	canShoot = true;

    for (int j = 0; j < hitResult.Num(); ++j)
    {
        if (hitResult[j].GetActor() != targetActor)
        {
            UPhysicalMaterial* PhysicsMtl = hitResult[j].PhysMaterial.Get();
            if (PhysicsMtl != nullptr)
            {
                if (PhysicsMtl->SurfaceType == SURFACE_TYPE_WOOD)
                {
                    canSee = false;
                }
                else if (PhysicsMtl->SurfaceType == SURFACE_TYPE_GLASS)
                {
                    canShoot = false;
                }
                else
                {
                    canSee = false;
                    canShoot = false;
                }
            }
        }
    }
	
}

void AExercice1::DrawCharacterAxes(UWorld * world, APawn * pawn) const
{
	FVector const pawnPosition = pawn->GetActorLocation();
	DrawDebugDirectionalArrow(world, pawnPosition, pawnPosition + pawn->GetActorRightVector() * 100.0f, 100, FColor::Green, false, -1.0f, 000, 5.0f);
	DrawDebugDirectionalArrow(world, pawnPosition, pawnPosition + pawn->GetActorForwardVector() * 100.0f, 100, FColor::Red, false, -1.0f, 000, 5.0f);
	DrawDebugDirectionalArrow(world, pawnPosition, pawnPosition + pawn->GetActorUpVector() * 100.0f, 100, FColor::Blue, false, -1.0f, 000, 5.0f);
}

void AExercice1::DrawVisionCone(UWorld * world, APawn * pawn, FVector const & dir, float angle) const
{
	FVector const pawnPosition = pawn->GetActorLocation();
	DrawDebugCone(world, pawnPosition, dir, 5000.0f, angle, angle, 50, FColor::Green, false, -1.0f, 0, 5.0f);
}

TArray<FOverlapResult> AExercice1::CollectTargetActorsInFrontOfCharacter(APawn const * pawn, PhysicsHelpers & physicHelper) const
{
	TArray<FOverlapResult> outResults;
	physicHelper.SphereOverlap(pawn->GetActorLocation() + pawn->GetActorForwardVector() * 750.0f, 1000, outResults, true);
	return outResults;
}

bool AExercice1::IsInsideCone(APawn * pawn, ATargetActor * targetActor) const
{
	FVector const toTarget = targetActor->GetActorLocation() - pawn->GetActorLocation();
	FVector const pawnForward = pawn->GetActorForwardVector();
	return std::abs(std::acos(FVector::DotProduct(pawnForward.GetSafeNormal(), toTarget.GetSafeNormal()))) < m_visionAngle;
}
