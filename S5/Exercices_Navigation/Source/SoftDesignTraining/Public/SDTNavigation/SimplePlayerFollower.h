// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimplePathfindingFollower.h"
#include "SimplePlayerFollower.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API ASimplePlayerFollower : public ASimplePathfindingFollower
{
	GENERATED_BODY()
	
    ACharacter* CharacterRef = nullptr;

    static bool DestinationSet;
    FVector CurrentDestination;

public:
    ASimplePlayerFollower();
	
    void GetPlayerRef();
    void PathFindToPlayer();

    FVector ComputeDestination(FVector playerPosition, FVector playerDirection);
    bool ValidateDestination(FVector destination);
    bool ShouldRePathToPlayer();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
