// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIBase.h"

#include "SoftDesignAIAvoidance.generated.h"

UCLASS(Blueprintable)
class SOFTDESIGNTRAINING_API ASoftDesignAIAvoidance : public AAIBase
{
    GENERATED_BODY()
public:

    void BeginPlay() override;
    // Called every frame
    void Tick(float DeltaTime) override;

    void ProcessAvoidance(float DeltaTime) override;
    void ProcessSimpleAvoidance(AActor* npcObstacle, float DeltaTime);
    void ProcessVOAvoidance(AActor* npcObstacle, float DeltaTime);

    // helper functions
    void ApplyNewVelocity(const FVector& newVelocity, float DeltaTime);
    void DisplayDebugAvoidance() const;
};
