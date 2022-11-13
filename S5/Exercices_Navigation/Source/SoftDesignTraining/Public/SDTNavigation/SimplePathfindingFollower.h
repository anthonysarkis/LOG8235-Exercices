// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SimplePathfindingFollower.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API ASimplePathfindingFollower : public APawn
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    bool IsWalking = true;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    float Acceleration = 300.f;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    float MaxSpeed = 300.f;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    float RotationRate = 100.f;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    TArray<AActor*> ActorsOfPath;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Options, meta = (AllowPrivateAccess = "true"))
    bool UseIntermediaryDestinations;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Options, meta = (AllowPrivateAccess = "true"))
    bool UseSlowDownForTurns;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Options, meta = (AllowPrivateAccess = "true"))
    bool UseShortcuts;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Debug, meta = (AllowPrivateAccess = "true"))
    bool ShowSpeedData;

    TArray<FVector> PathToFollow;
    int32           CurrentDestinationIndex = -1;
    float           CurrentSpeed = 0.f;
    FVector         Direction = FVector(0.f, 1.f, 0.f);

    bool            IsTurning = false;
    float           SlowDownTargetSpeed = -1.f;
    FVector         SlowDownStartPos = FVector::ZeroVector;
    int32           IndexAfterSlowDown = -1;

public:
	// Sets default values for this pawn's properties
	ASimplePathfindingFollower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    FVector ComputeDestination(float DeltaTime);
    void    UpdateDirection(float DeltaTime, FVector directionGoal);
    FVector ComputeVelocity(float DeltaTime, FVector destination);
    float   ComputeTargetSpeedForTurn();
    void    ApplyVelocity(float DeltaTime, FVector velocity);

	// Exercise function to complete
    void    UseIntermediaryDestination_Behavior(FVector2D position2D, FVector2D destination2D, FVector& destination);
    float   UseSlowDownForTurns_Behavior(FVector destination, float deltaTime);
    void    UseShortcuts_Behavior(FVector2D& destination2D, FVector& destination);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
