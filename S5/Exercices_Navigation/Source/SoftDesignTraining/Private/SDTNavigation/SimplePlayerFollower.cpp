// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTNavigation/SimplePlayerFollower.h"
#include "SoftDesignTraining.h"
#include <NavigationSystem.h>
#include <NavigationPath.h>
//#include <Navigation/NavigationTypes.h>
//#include <Navigation/NavigationData.h>
#include <DrawDebugHelpers.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <Engine/Engine.h>
#include "SDTUtils.h"

#define REPATH_THRESHOLD 200.f

bool ASimplePlayerFollower::DestinationSet = false;



// -----------------------------------------------------------------------------------------------------------
// ------------------------------------------------ EXERCICES ------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// EXERCICE 1
///////////////////////////////////////////////////////////////////////////////
//
// �nonc� :
//    - Trouver la destination permettant de suivre le joueur
//
//    - La meilleure destination se trouve derri�re le joueur � une certaine distance
//    - Les unit�s dans Unreal sont en cm (100.f = 1 m�tre)
//
///////////////////////////////////////////////////////////////////////////////
FVector ASimplePlayerFollower::ComputeDestination(FVector playerPosition, FVector playerDirection)
{
    return playerPosition;
}

///////////////////////////////////////////////////////////////////////////////
// EXERCICE 2
///////////////////////////////////////////////////////////////////////////////
//
// �nonc� :
//    - V�rifier la validit� de la destination
//
//    - Pour �tre valide, la destination doit se trouver sur le navmesh
//
//    - Pour verifier si un point est sur le navmesh, utilisez SDTUtils::RaycastNavMesh
//      en dirigeant le raycast vers le bas
//
//    - La fonction renvoie true si le raycast touche le navmesh et false sinon
//
//    - Un message affichant "INVALID DESTINATION USED" s'affiche si une destination
//      invalide est quand m�me valid�e par cette fonction
//
///////////////////////////////////////////////////////////////////////////////
bool ASimplePlayerFollower::ValidateDestination(FVector destination)
{
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// EXERCICE 3
///////////////////////////////////////////////////////////////////////////////
//
// �nonc� :
//    - Impl�menter la condition permettant le demande de recalcul du chemin vers le joueur
//
//    - Pour l'instant, elle renvoie true � chaque appel ce qui signifie qu'un agent
//      reconstruit son chemin � toute les frames
//
///////////////////////////////////////////////////////////////////////////////
bool ASimplePlayerFollower::ShouldRePathToPlayer()
{
    return true;
}

// -----------------------------------------------------------------------------------------------------------
// -------------------------------------------- FIN DES EXERCICES --------------------------------------------
// -----------------------------------------------------------------------------------------------------------



ASimplePlayerFollower::ASimplePlayerFollower() :
ASimplePathfindingFollower()
{

}

// Called when the game starts or when spawned
void ASimplePlayerFollower::BeginPlay()
{
    Super::BeginPlay();

    GetPlayerRef();

    PathFindToPlayer();
}

void ASimplePlayerFollower::GetPlayerRef()
{
    ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if(character)
    {
        CharacterRef = character;
    }
    else
    {
        FTimerHandle dummyHandler;
        GetWorld()->GetTimerManager().SetTimer(dummyHandler, FTimerDelegate::CreateUObject(this, &ASimplePlayerFollower::GetPlayerRef), 2.f, false);
    }
}

void ASimplePlayerFollower::PathFindToPlayer()
{
    if(!CharacterRef)
        return;

    FVector destination;
    if(!DestinationSet)
    {
        FVector playerDirection = CharacterRef->GetActorForwardVector();
        playerDirection.Z = 0.f;
        playerDirection.Normalize();
        destination = ComputeDestination(CharacterRef->GetActorLocation(), playerDirection);

        if(!ValidateDestination(destination))
            return;
    }
    else
    {
        destination = CurrentDestination;
    }

    UNavigationPath* path = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetActorLocation(), destination);
    if(!path || !path->GetPath().IsValid() || path->GetPath()->IsPartial() || path->GetPath()->GetPathPoints().Num() == 0)
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("INVALID DESTINATION USED"));
        return;
    }

    CurrentDestination = destination;
    DestinationSet = true;

    PathToFollow.Empty();
    IsWalking = true;

    for(FNavPathPoint& point : path->GetPath()->GetPathPoints())
        PathToFollow.Push(point.Location);

    CurrentDestinationIndex = 0;
}

void ASimplePlayerFollower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(CharacterRef)
    {
        if(ShouldRePathToPlayer())
        {
            DestinationSet = false;
            PathFindToPlayer();
        }
    }
}
