#pragma once

#include "CoreMinimal.h"
#include "PatrolActions.h"
#include "AIBase.h"
#include "AIPatrol.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API AAIPatrol : public AAIBase
{
    GENERATED_BODY()

public:
    AAIPatrol();

    virtual void Tick(float DeltaSeconds) override;
    virtual void ProcessMovement(float DeltaTime) override;
    virtual void SetMaxWalkingSpeed() override;
    virtual void SetInitialPatrolling() override;
    virtual void DetectPlayer() override;
    virtual void UpdateNextDestination() override;
    virtual bool HasReachedDestination(const FVector actorLocation) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AIPatrol")
    AActor* PatrolPathActor;

protected:
    float m_LastFrameSpeed;
    float m_TargetSpeed;
    float m_LinearSpeedChangeTime;
    float m_LinearSpeedChangeTimer;
    float m_InitialSpeed;
    float m_StopTimer;
    float m_WaitTime;
    UPatrolLayeredAction m_LayeredAction;

    void ProceedToNextDesination();

private:
    void SetupSpeedTransition();
    void SetDestinationData(const AActor& waypointActor);
    AActor* GetWayPointAtIndex(int32 index);
    void UpdateReachedDestination();
    virtual void UpdateNavigation(float DeltaTime);
    virtual void UpdateCurrentSpeed(float DeltaTime);
    

    virtual int GetNoExercice() { return 0; }

    virtual void UpdateNavigation_Exercice1(float DeltaTime, float speed) {};
    virtual void UpdateNavigation_Exercice2(float DeltaTime, float currentSpeed, float targetSpeed, float& outNewSpeed) {};
    virtual void UpdateLayeredAction_Exercice2(UPatrolLayeredAction layeredAction) {};
    virtual void UpdateNavigation_Exercice3(float DeltaTime, float currentSpeed, float targetSpeed, float& outNewSpeed, FVector currentPosition, FVector stopPosition) {};

};