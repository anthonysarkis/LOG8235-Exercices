// LOG8235 - August 2020

#include "Exo1/Exercice1.h"
#include "SoftDesignTraining.h"
#include <cmath>
#include "DrawDebugHelpers.h"
#include "TargetActor.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void AExercice1::GetVisibilityInformationForTarget(ATargetActor* targetActor, FVector selfPosition, PhysicsHelpers& physicHelper, bool& canSee, bool& canShoot)
{
}

void AExercice1::DrawCharacterAxes(UWorld * world, APawn * pawn) const
{
}

void AExercice1::DrawVisionCone(UWorld * world, APawn * pawn, FVector const & dir, float angle) const
{
}

TArray<FOverlapResult> AExercice1::CollectTargetActorsInFrontOfCharacter(APawn const * pawn, PhysicsHelpers & physicHelper) const
{
	return TArray<FOverlapResult>();
}

bool AExercice1::IsInsideCone(APawn * pawn, ATargetActor * targetActor) const
{
	return false;
}
