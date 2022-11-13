// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "PatrolActions.h"
#include "WayPointComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOFTDESIGNTRAINING_API UWayPointComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWayPointComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TargetSpeed = 200.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TimeToReachSpeed = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TimeToStop = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool IsADirectionChange = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WaitTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UPatrolLayeredAction LayeredAction = UPatrolLayeredAction::None;
};
