// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "WayPointComponent.h"
#include "PatrolPathComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOFTDESIGNTRAINING_API UPatrolPathComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolPathComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	unsigned long GetNWayPoints();
	void GetNextWayPoint(long& wayPoint, UWayPointComponent*& nextWaypoint, long direction = 1);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AIPatrol")
    TArray<AActor *> m_PatrolPoints;
};
