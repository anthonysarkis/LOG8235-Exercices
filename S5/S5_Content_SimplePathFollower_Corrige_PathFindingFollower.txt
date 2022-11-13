//////////////////////////////////////////////////////
// CORRIGÉ EXERCICE 1
//////////////////////////////////////////////////////

void ASimplePathfindingFollower::UseShortcuts_Behavior(FVector2D& destination2D, FVector& destination)
{
    if (CurrentDestinationIndex != PathToFollow.Num() - 1)
    {
        TArray<FVector> sampleShortcutPoints;

        sampleShortcutPoints.Empty();
        sampleShortcutPoints.Reserve(SHORTCUT_SAMPLE_NUM);
        FVector startNextPath = PathToFollow[CurrentDestinationIndex];
        FVector endNextPath = PathToFollow[CurrentDestinationIndex + 1];

        FVector displacementSamples = (endNextPath - startNextPath) / SHORTCUT_SAMPLE_NUM;

        for (int i = 0; i < SHORTCUT_SAMPLE_NUM; i++)
        {
            FVector sample = startNextPath + ((i + 1) * displacementSamples);
            sampleShortcutPoints.Push(sample);
#ifdef PATH_FOLLOW_DEBUG
            {
                DrawDebugSphere(
                    GetWorld(),
                    sample,
                    10.f, 20,
                    FColor::Emerald,
                    false, -1, 0
                );
            }
#endif //PATH_FOLLOW_DEBUG
        }

        FVector originBB = FVector::ZeroVector;
        FVector extentsBB;
        GetActorBounds(true, originBB, extentsBB);
        extentsBB.Z = 0.f;

        for (int i = SHORTCUT_SAMPLE_NUM - 1; i >= 0; i--)
        {
            FVector start = GetActorLocation();
            start.Z = originBB.Z;
            FVector end = sampleShortcutPoints[i];
            end.Z = originBB.Z;

            FVector sideVectorLeft = FVector::CrossProduct((end - start), GetActorUpVector());
            sideVectorLeft.Normalize();
            sideVectorLeft *= extentsBB.Size();
            FVector sideVectorRight = -sideVectorLeft;

            FHitResult HitOut;
            // EXERCICE 3 : TESTS RAYCASTS
            if(!SDTUtils::Raycast(GetWorld(),
                start + sideVectorLeft,	//start
                end + sideVectorLeft //end
                )
                &&
                !SDTUtils::Raycast(GetWorld(),
                    start + sideVectorRight,	//start
                    end + sideVectorRight //end
                )
                )
            {
                destination = sampleShortcutPoints[i];
                destination2D = FVector2D(sampleShortcutPoints[i]);
                if (destination.Equals(PathToFollow[CurrentDestinationIndex + 1]))
                    CurrentDestinationIndex++;
                break;
            }
        }
    }
}

//////////////////////////////////////////////////////
// CORRIGÉ EXERCICE 2
//////////////////////////////////////////////////////

void ASimplePathfindingFollower::UseIntermediaryDestination_Behavior(FVector2D position2D, FVector2D destination2D, FVector& destination)
{
    FVector lineStart = PathToFollow[CurrentDestinationIndex - 1];
    FVector lineEnd = PathToFollow[CurrentDestinationIndex];
    FVector lineDirection = lineEnd - lineStart;
    lineDirection.Normalize();

    FVector projectedPosition = FMath::ClosestPointOnLine(lineStart, lineEnd, GetActorLocation());

    if((GetActorLocation() - lineEnd).Size() > 1.f)
        destination = projectedPosition + ((lineEnd - projectedPosition) / 2);
}

//////////////////////////////////////////////////////
// CORRIGÉ EXERCICE 3
//////////////////////////////////////////////////////

float ASimplePathfindingFollower::ComputeTargetSpeedForTurn()
{
    if(CurrentDestinationIndex == PathToFollow.Num() - 1)
        return -1.f;

    FVector nextDirection = PathToFollow[CurrentDestinationIndex + 1] - PathToFollow[CurrentDestinationIndex];
    nextDirection.Normalize();

    float speedFactor = 1.f;

    float cosAngleDir = FVector::DotProduct(Direction, nextDirection);
    speedFactor = (cosAngleDir + 1) / 2;

    return MaxSpeed * speedFactor;
}