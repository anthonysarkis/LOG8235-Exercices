// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ReactionEvent.h"
#include "WayPointComponent.h"
#include "AIBase.generated.h"

class UPatrolPathComponent;

/**
* Tick function that calls AAIBase::PostPhysicsTickComponent
**/
USTRUCT()
struct FAIBasePostPhysicsTickFunction : public FTickFunction
{
    GENERATED_USTRUCT_BODY()

        /** CharacterMovementComponent that is the target of this tick **/
        class AAIBase* Target;

    /**
    * Abstract function actually execute the tick.
    * @param DeltaTime - frame time to advance, in seconds
    * @param TickType - kind of tick for this frame
    * @param CurrentThread - thread we are executing on, useful to pass along as new tasks are created
    * @param MyCompletionGraphEvent - completion event for this task. Useful for holding the completion of this task until certain child tasks are complete.
    **/
    virtual void ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;

    /** Abstract function to describe this tick. Used to print messages about illegal cycles in the dependency graph **/
    virtual FString DiagnosticMessage() override;
};

template<>
struct TStructOpsTypeTraits<FAIBasePostPhysicsTickFunction> : public TStructOpsTypeTraitsBase2<FAIBasePostPhysicsTickFunction>
{
    enum
    {
        WithCopy = false
    };
};

UCLASS(Blueprintable)
class SOFTDESIGNTRAINING_API AAIBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
    AAIBase(const FObjectInitializer& ObjectInitializer);
    AAIBase();

    virtual void PostInitProperties() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	//virtual void EndPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    /** Post-physics tick function for this character */
    UPROPERTY()
    struct FAIBasePostPhysicsTickFunction PostPhysicsTickFunction;

    /** Tick function called after physics (sync scene) has finished simulation, before cloth */
    virtual void PrePhysicsTickComponent(float DeltaTime);

    /** Tick function called after physics (sync scene) has finished simulation, before cloth */
    virtual void PostPhysicsTickComponent(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void SetNewMoveDestination(const FVector DestLocation);
	virtual void ProcessReaction( ReactionEvent* reactionEvent);
	virtual void ProcessAvoidance();
    virtual void ProcessMovement(float DeltaTime);
    virtual void DetectPlayer();
    virtual void SetInitialPatrolling();
    virtual void SetMaxWalkingSpeed();
    virtual void UpdateNextDestination();
    virtual bool HasReachedDestination(const FVector actorLocation);

    //Exercices
    virtual void ProcessLookAt_Exercice4(FVector npcPosition, FVector targetPosition, FTransform headTransformWorldSpace, float deltaTime, FTransform& outHeadTransform);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AiBase")
    bool m_IsBlind;
    bool m_IsLooking;
    bool m_IsPlayerDetected;
    long m_CurrentWayPoint;
    long m_PathDirection;
    UPatrolPathComponent* m_PatrolPath;
    UWayPointComponent* m_NextWaypoint;
    FVector m_NextDestination;
    FVector m_CurrentDestination;

    float m_LookAtYawValue;
    float m_LookAtPitchValue;

    FQuat m_rotationOffsetBetweenNeckAndHead;
    FQuat m_rotationOffsetBetweenNeckAndHead_T;

    virtual bool ProcessBlindness(float DeltaTime);
    virtual bool ProcessPlayerDetection(float DeltaTime);
    virtual void ProcessLookAt(float DeltaTime);

    virtual FTransform GetNPCTransform();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AiBase")
    bool m_IsExo4Hack;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AiBase")
    class UPoseableMeshComponent* modifiableMesh;


private:
	float m_BlindTimeLeft;
    float m_LookTimeLeft;
    FVector m_LookPosition;


};
