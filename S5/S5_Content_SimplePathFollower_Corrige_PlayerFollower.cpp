//////////////////////////////////////////////////////
// CORRIGÉ EXERCICE 1
//////////////////////////////////////////////////////

FVector ASimplePlayerFollower::ComputeDestination(FVector playerPosition, FVector playerDirection)
{
    float offset = 100.f;

    return (playerPosition - playerDirection * offset);
}

//////////////////////////////////////////////////////
// CORRIGÉ EXERCICE 2
//////////////////////////////////////////////////////

bool ASimplePlayerFollower::ValidateDestination(FVector destination)
{
    return SDTUtils::RaycastNavMesh(GetWorld(), destination, destination - FVector(0.f, 0.f, 100.f));
}


//////////////////////////////////////////////////////
// CORRIGÉ EXERCICE 3
//////////////////////////////////////////////////////

bool ASimplePlayerFollower::ShouldRePathToPlayer()
{
    if((CharacterRef->GetActorLocation() - CurrentDestination).SizeSquared() > REPATH_THRESHOLD * REPATH_THRESHOLD)
        return true;

    return false;
}