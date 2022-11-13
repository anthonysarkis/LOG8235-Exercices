// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTNavigation/SimplePathfindingFollower.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "SDTUtils.h"
#include <Engine/Engine.h>

#define PATH_FOLLOW_DEBUG
#define SHORTCUT_SAMPLE_NUM 8



// -----------------------------------------------------------------------------------------------------------
// ------------------------------------------------ EXERCICES ------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// EXERCICE 1
//////////////////////////////////////////////////////////////////////////////
//
// �nonc� :
//    - on veut d�finir les deux raycasts permettant de savoir si un agent peut
//      atteindre un point choisi sur la prochaine portion du chemin
//
//    - les raycasts doivent �tre faits � droite et � gauche de l'agent pour
//      prendre en compte sa largeur
//
//    - pour effectuer un raycast utiliser SDTUtils::Raycast(UWorld* world, FVector start, FVector end)
//      utilisez GetWorld() pour le param�tre world
//      la fonction retourne vrai si une collision est d�tect�e
//
//    - la droite et la gauche de l'agent d�pendent de son orientation
//
/////////////////////////////////////////////////////////////////////////////
void ASimplePathfindingFollower::UseShortcuts_Behavior(FVector2D& destination2D, FVector& destination)
{
    if(CurrentDestinationIndex != PathToFollow.Num() - 1)
    {
        TArray<FVector> sampleShortcutPoints;

        sampleShortcutPoints.Empty();
        sampleShortcutPoints.Reserve(SHORTCUT_SAMPLE_NUM);
        FVector startNextPath = PathToFollow[CurrentDestinationIndex];
        FVector endNextPath = PathToFollow[CurrentDestinationIndex + 1];

        FVector displacementSamples = (endNextPath - startNextPath) / SHORTCUT_SAMPLE_NUM;

        for(int i = 0; i < SHORTCUT_SAMPLE_NUM; i++)
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

        for(int i = SHORTCUT_SAMPLE_NUM - 1; i >= 0; i--)
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
            // EXERCICE 1 : TESTS RAYCASTS
            if(false
               &&
               false
              )
            {
                destination = sampleShortcutPoints[i];
                destination2D = FVector2D(sampleShortcutPoints[i]);
                if(destination.Equals(PathToFollow[CurrentDestinationIndex + 1]))
                    CurrentDestinationIndex++;
                break;
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// EXERCICE 2
//////////////////////////////////////////////////////////////////////////////
//
// �nonc� :
//    - on veut prendre comme destination le milieu du segment form� par le noeud de destination
//      et la projection de la position de l'agent sur le segment du chemin.
//
//    - Il faudra modifier la valeur du param�tre destination pour changer celle-ci.
//
//    - On fournit de base les points d�finissant le segment du chemin et la direction de celui-ci.
//
//    - Pour projeter un point sur un segment utilisez FMath::ClosestPointOnLine(const FVector& LineStart, const FVector& LineEnd, const FVector& Point).
//    - Pour trouver la position de l'agent utilisez GetActorLocation().
//
/////////////////////////////////////////////////////////////////////////////
void ASimplePathfindingFollower::UseIntermediaryDestination_Behavior(FVector2D position2D, FVector2D destination2D, FVector& destination)
{
    FVector lineStart = PathToFollow[CurrentDestinationIndex - 1];
    FVector lineEnd = PathToFollow[CurrentDestinationIndex];
    FVector lineDirection = lineEnd - lineStart;
    lineDirection.Normalize();

    // EXERCICE 2 : Modifier le param�tre destination ici pour choisir le point interm�diaire
}

///////////////////////////////////////////////////////////////////////////////
// EXERCICE 3
//////////////////////////////////////////////////////////////////////////////
//
// �nonc� :
//    - on veut trouver le facteur de d�c�laration qui va nous permettre de trouver la
//      vitesse � atteindre pour pouvoir tourner au virage sans trop s'�loigner du chemin
//
//    - le facteur doit �tre entre 0.f et 1.f
//
//    - le facteur est 1.f lorsque le prochain chemin est parfaitement align� avec le notre
//      et continue dans la m�me direction
//
//    - � l'inverse, le facteur est 0.f lorsque le prochain chemin est dans le sens
//      strictement inverse � notre chemin (demi tour)
//
//    - Pour trouver ce facteur n'h�sitez pas � vous servir de l'angle entre le chemin
//      actuel et le prochain. On peut faire un produit vectoriel avec FVector::DotProduct().
//
/////////////////////////////////////////////////////////////////////////////
float ASimplePathfindingFollower::ComputeTargetSpeedForTurn()
{
    if(CurrentDestinationIndex == PathToFollow.Num() - 1)
        return -1.f;

    FVector nextDirection = PathToFollow[CurrentDestinationIndex + 1] - PathToFollow[CurrentDestinationIndex];
    nextDirection.Normalize();

    float speedFactor = 1.f;

    // EXERCICE 3 : REMPLIR ICI

    return MaxSpeed * speedFactor;
}

// -----------------------------------------------------------------------------------------------------------
// -------------------------------------------- FIN DES EXERCICES --------------------------------------------
// -----------------------------------------------------------------------------------------------------------



float ASimplePathfindingFollower::UseSlowDownForTurns_Behavior(FVector destination, float deltaTime)
{
    // On calcule une fois la vitesse auquelle on veut descendre arriv� au prochain noeud
    if (!IsTurning && SlowDownTargetSpeed == -1.f)
    {
        SlowDownTargetSpeed = ComputeTargetSpeedForTurn();
        FVector destToPos = GetActorLocation() - destination;
        destToPos.Normalize();
        SlowDownStartPos = destination + destToPos * 200.f;
        IndexAfterSlowDown = CurrentDestinationIndex + 1;
    }

    if (!IsTurning)
        return FMath::Lerp(MaxSpeed, SlowDownTargetSpeed, 1 - ((destination - GetActorLocation()).Size() / (destination - SlowDownStartPos).Size()));
    else
        return SlowDownTargetSpeed;
}

// Sets default values
ASimplePathfindingFollower::ASimplePathfindingFollower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASimplePathfindingFollower::BeginPlay()
{
	Super::BeginPlay();
	
    if(ActorsOfPath.Num() > 0)
    {
        for(int32 i = 0; i < ActorsOfPath.Num(); i++)
            PathToFollow.Add(ActorsOfPath[i]->GetActorLocation());

        CurrentDestinationIndex = 0;
    }
}

FVector ASimplePathfindingFollower::ComputeDestination(float DeltaTime)
{
    FVector2D position2D(GetActorLocation());
    FVector destination(PathToFollow[CurrentDestinationIndex]);
    FVector2D destination2D(destination);

    if (UseShortcuts)
        UseShortcuts_Behavior(destination2D, destination);

    if(position2D.Equals(destination2D, 10.f))
    {
        CurrentDestinationIndex++;

        if(CurrentDestinationIndex == PathToFollow.Num())
        {
            CurrentDestinationIndex = -1;
            destination = FVector(position2D, 0.f) + Direction;
        }
        else
        {
            destination = PathToFollow[CurrentDestinationIndex];
        }
    }

    if(UseIntermediaryDestinations && !position2D.Equals(destination2D, 20.f) && CurrentDestinationIndex > 0)
        UseIntermediaryDestination_Behavior(position2D, destination2D, destination);

    return destination;
}

void ASimplePathfindingFollower::UpdateDirection(float DeltaTime, FVector directionGoal)
{
    directionGoal.Z = 0.f;

    if(!FMath::IsNearlyEqual(directionGoal.SizeSquared(), 1))
        directionGoal.Normalize();

    if(Direction.Equals(directionGoal, 0.01f))
    {
        Direction = directionGoal;
        IsTurning = false;
        if(CurrentDestinationIndex == IndexAfterSlowDown)
            SlowDownTargetSpeed = -1.f;
        return;
    }

    IsTurning = true;

    FRotator currentRotation = Direction.ToOrientationRotator();
    FRotator goalRotation = directionGoal.ToOrientationRotator();

    FRotator deltaRotation = goalRotation - currentRotation;

    if(deltaRotation.Yaw > 180.f)
        deltaRotation.Yaw -= 360.f;

    if(deltaRotation.Yaw < -180.f)
        deltaRotation.Yaw += 360.f;

    float RotationThisFrame = RotationRate * DeltaTime;

    deltaRotation.Yaw = FMath::Clamp(deltaRotation.Yaw, -RotationThisFrame, RotationThisFrame);

    Direction = deltaRotation.RotateVector(Direction);
}

FVector ASimplePathfindingFollower::ComputeVelocity(float DeltaTime, FVector destination)
{
    FVector velocity = FVector::ZeroVector;
    float speed = MaxSpeed;

    if (UseSlowDownForTurns && (IsTurning || (destination - GetActorLocation()).Size() < 200.f) && CurrentDestinationIndex != -1 && CurrentDestinationIndex != PathToFollow.Num() - 1)
        speed = UseSlowDownForTurns_Behavior(destination, DeltaTime);

    if(ShowSpeedData)
    {
	    GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Actual Speed : %f"), speed));
	    GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Slowdown Target Speed : %f"), SlowDownTargetSpeed));
    }

    CurrentSpeed = FMath::Clamp(CurrentSpeed + (Acceleration * DeltaTime), 0.f, speed);
    velocity = Direction * CurrentSpeed;

    return velocity;
}

void ASimplePathfindingFollower::ApplyVelocity(float DeltaTime, FVector velocity)
{
    if(!velocity.IsNearlyZero())
    {
        FVector position = GetActorLocation();

        FVector destination = position + velocity * DeltaTime;

        SetActorLocation(destination);
    }
}

// Called every frame
void ASimplePathfindingFollower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    #ifdef PATH_FOLLOW_DEBUG
    {
        FVector position = GetActorLocation();
        position.Z += 50;

        DrawDebugLine(
            GetWorld(),
            position,
            position + Direction * 100.f,
            FColor(255, 0, 0),
            false, -1, 0,
            12.333
        );

        for(int i = 0; i < PathToFollow.Num() - 1; i++)
            DrawDebugLine(
                GetWorld(),
                PathToFollow[i],
                PathToFollow[i + 1],
                FColor(0, 0, 255),
                false, -1, 0,
                5.f
            );
    }
    #endif //PATH_FOLLOW_DEBUG

    if(IsWalking && PathToFollow.Num() > 0)
    {
        FVector velocity = FVector::ZeroVector;

        if(CurrentDestinationIndex > -1 && CurrentDestinationIndex < PathToFollow.Num())
        {
            FVector position = GetActorLocation();
            FVector destination = ComputeDestination(DeltaTime);

#ifdef PATH_FOLLOW_DEBUG
            {
                DrawDebugSphere(
                    GetWorld(),
                    destination,
                    20.f, 20,
                    FColor(255, 0, 0),
                    false, -1, 0
                );
            }
#endif //PATH_FOLLOW_DEBUG

            UpdateDirection(DeltaTime, destination - position);
            velocity = ComputeVelocity(DeltaTime, destination);

            if(ShowSpeedData)
			    GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Current Speed : %f"), CurrentSpeed));
        }
        else
        {
            velocity = ComputeVelocity(-DeltaTime, GetActorLocation() + Direction);

            if(velocity.SizeSquared() < 0.1f)
            {
                CurrentSpeed = 0.f;
                CurrentDestinationIndex = -1.f;
                IsWalking = false;
            }
        }

        ApplyVelocity(DeltaTime, velocity);
    }
}

// Called to bind functionality to input
void ASimplePathfindingFollower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

