// Fill out your copyright notice in the Description page of Project Settings.

#include "AIBase.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "ReactionManager.h"
#include "PatrolPathManager.h"
#include "PatrolPathComponent.h"
#include "ObjectPartition.h"
#include "SDTUtils.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UObject/UObjectGlobals.h"
#include "Components/PoseableMeshComponent.h"

void FAIBasePostPhysicsTickFunction::ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
    if (Target && !Target->HasAnyFlags(RF_BeginDestroyed | RF_NeedInitialization))
    {
        FScopeCycleCounterUObject ActorScope(Target);
        Target->PostPhysicsTickComponent(DeltaTime*Target->CustomTimeDilation);
    }
}

FString FAIBasePostPhysicsTickFunction::DiagnosticMessage()
{
    return Target->GetFullName() + TEXT("[AAIBase::PostPhysicsTick]");
}

// Sets default values
AAIBase::AAIBase(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
     // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

    PrimaryActorTick.bCanEverTick = true;
    m_CurrentWayPoint = 0;
    m_PatrolPath = nullptr;
    
    m_IsBlind = false;
    m_IsPlayerDetected = false;

    m_BlindTimeLeft = 0.0f;
    m_LookTimeLeft = 0.0f;
    m_IsLooking = false;
    m_LookPosition = FVector::ZeroVector;

    FRotator headDirectionCompRotOffset = FRotator::ZeroRotator;
    headDirectionCompRotOffset.Pitch = 90.0f;
    headDirectionCompRotOffset.Roll = 90.0f;
    headDirectionCompRotOffset.Yaw = 90.0f;
    m_rotationOffsetBetweenNeckAndHead_T = headDirectionCompRotOffset.Quaternion();
    m_rotationOffsetBetweenNeckAndHead_T.Normalize();

    headDirectionCompRotOffset.Pitch = 90.0f;
    headDirectionCompRotOffset.Roll = 90.0f;
    headDirectionCompRotOffset.Yaw = 90.0f;
    m_rotationOffsetBetweenNeckAndHead = headDirectionCompRotOffset.Quaternion();
    m_rotationOffsetBetweenNeckAndHead.Normalize();

    m_IsExo4Hack = false;

    PostPhysicsTickFunction.bCanEverTick = true;
    PostPhysicsTickFunction.bStartWithTickEnabled = true;
    PostPhysicsTickFunction.TickGroup = TG_PostPhysics;
}

AAIBase::AAIBase()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

    PrimaryActorTick.bCanEverTick = true;
    m_CurrentWayPoint = 0;
    m_PatrolPath = nullptr;

    m_IsBlind = false;
    m_IsPlayerDetected = false;

    m_BlindTimeLeft = 0.0f;
    m_LookTimeLeft = 0.0f;
    m_IsLooking = false;
    m_LookPosition = FVector::ZeroVector;

    m_LookAtYawValue = 0.0f;
    m_LookAtPitchValue = 0.0f;

    FRotator headDirectionCompRotOffset = FRotator::ZeroRotator;
    headDirectionCompRotOffset.Pitch = 90.0f;
    headDirectionCompRotOffset.Roll = 90.0f;
    headDirectionCompRotOffset.Yaw = 90.0f;
    m_rotationOffsetBetweenNeckAndHead_T = headDirectionCompRotOffset.Quaternion();
    m_rotationOffsetBetweenNeckAndHead_T.Normalize();

    headDirectionCompRotOffset.Pitch = 90.0f;
    headDirectionCompRotOffset.Roll = 90.0f;
    headDirectionCompRotOffset.Yaw = 90.0f;
    m_rotationOffsetBetweenNeckAndHead = headDirectionCompRotOffset.Quaternion();
    m_rotationOffsetBetweenNeckAndHead.Normalize();

    m_IsExo4Hack = false;

    PostPhysicsTickFunction.bCanEverTick = true;
    PostPhysicsTickFunction.bStartWithTickEnabled = true;
    PostPhysicsTickFunction.TickGroup = TG_PostPhysics;
}

void AAIBase::PostInitProperties()
{
    Super::PostInitProperties();

    if (!IsTemplate() && PostPhysicsTickFunction.bCanEverTick)
    {
        PostPhysicsTickFunction.Target = this;
        PostPhysicsTickFunction.SetTickFunctionEnable(PostPhysicsTickFunction.bStartWithTickEnabled);
        PostPhysicsTickFunction.RegisterTickFunction(GetLevel());
    }
}

// Called when the game starts or when spawned
void AAIBase::BeginPlay()
{
    Super::BeginPlay();

    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        reactionManager->RegisterNPC(this);
    }

    SetMaxWalkingSpeed();

    SetInitialPatrolling();
}


void AAIBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        reactionManager->UnregisterNPC(this);
    }

    m_PatrolPath = nullptr;
}

// Called every frame
void AAIBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //return;
    PrePhysicsTickComponent(DeltaTime);
}

void AAIBase::PrePhysicsTickComponent(float DeltaTime)
{
    if (USkeletalMeshComponent* SkelMesh = GetMesh())
    {
        SkelMesh->SetComponentSpaceTransformsDoubleBuffering(false);

        TArray<USkeletalMeshComponent*> SkelMeshes;
        GetComponents<USkeletalMeshComponent>(SkelMeshes);
        if (SkelMeshes.Num() > 1) {
            SkelMeshes[0]->SetComponentSpaceTransformsDoubleBuffering(true);
            SkelMeshes[1]->SetComponentSpaceTransformsDoubleBuffering(true);
        }
    }
    //check(SkelMesh);
    // to be able to overwrite bones in post-physics update


    DetectPlayer();
    ProcessMovement(DeltaTime);
    ProcessAvoidance();
}

void AAIBase::PostPhysicsTickComponent(float DeltaTime)
{
    ProcessLookAt(DeltaTime);
}

bool AAIBase::ProcessBlindness(float DeltaTime)
{
    if (m_BlindTimeLeft > 0)
    {
        m_BlindTimeLeft -= DeltaTime;
        SetNewMoveDestination(GetActorLocation());
        return false;
    }
    else
    {
        m_BlindTimeLeft = 0.0f;
        m_IsBlind = false;

        UpdateNextDestination();
    }

    return true;
}

FTransform AAIBase::GetNPCTransform()
{
	FTransform npcTransform = GetActorTransform();

    // Compensating a data defect that added a 90 degree rotation on the model component
	FRotator npcCompRotOffset = FRotator::ZeroRotator;
	npcCompRotOffset.Yaw = 90.0f;

	npcTransform.SetRotation(npcTransform.GetRotation() * npcCompRotOffset.Quaternion());

	return npcTransform;
}

void AAIBase::ProcessLookAt(float DeltaTime)
{
    AActor* targetPlayer = nullptr;
    ReactionManager* reactionManager = ReactionManager::GetInstance();

    if (reactionManager)
    {
        targetPlayer = reactionManager->GetPlayer();
    }

    const FName boneName = TEXT("Head");

    if (targetPlayer)
    {
        if (UPoseableMeshComponent* pMeshComponent = FindComponentByClass<UPoseableMeshComponent>())
        {
            FTransform SpaceBasesDataHead = pMeshComponent->GetBoneTransformByName(boneName, EBoneSpaces::ComponentSpace);

            // Acquire target location
            FVector targetPosition = targetPlayer->GetActorLocation();
            FVector npcPosition = GetActorLocation();

            FTransform headSpaceWorld;
            headSpaceWorld.SetTranslation(pMeshComponent->GetBoneLocation(boneName, EBoneSpaces::WorldSpace));
            headSpaceWorld.SetRotation(pMeshComponent->GetBoneQuaternion(boneName, EBoneSpaces::WorldSpace));

            ProcessLookAt_Exercice4(npcPosition, targetPosition, headSpaceWorld, DeltaTime, SpaceBasesDataHead);
            pMeshComponent->SetBoneTransformByName(boneName, SpaceBasesDataHead, EBoneSpaces::ComponentSpace);
        }

    }
   
}

void AAIBase::ProcessLookAt_Exercice4(FVector npcPosition, FVector targetPosition, FTransform headTransformWorldSpace, float DeltaTime, FTransform& SpaceBasesDataHead)
{
    return;
}

bool AAIBase::ProcessPlayerDetection(float DeltaTime)
{
    AActor* targetPlayer = NULL;

    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        targetPlayer = reactionManager->GetPlayer();
    }

    if (targetPlayer)
    {
        FVector npcPosition = GetActorLocation();
        FVector targetPosition = targetPlayer->GetActorLocation();
        FVector distToTarget = npcPosition - targetPosition;
        if (distToTarget.Size() < 250.0f)
        {
            SetNewMoveDestination(GetActorLocation());
        }
        else
        {
            SetNewMoveDestination(targetPosition);
        }

        m_CurrentDestination = FVector::ZeroVector;
        return false;
    }

    return true;
}

// Called to bind functionality to input
void AAIBase::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
    Super::SetupPlayerInputComponent(inputComponent);
}

void AAIBase::SetNewMoveDestination(const FVector DestLocation)
{
    // TODO: MAYBE ACTIVATE MOVE TO LOCATION OF NAV SYSTEM AGAIN? 
    //const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    //NavSys->SimpleMoveToLocation(GetController(), DestLocation);

    UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), DestLocation);
}

void AAIBase::ProcessReaction(ReactionEvent* reactionEvent)
{
    if (!reactionEvent)
        return;

    FVector npcPosition = GetActorLocation();
    FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
    UWorld* npcWorld = GetWorld();

    //DrawDebugSphere(npcWorld, npcHead, reactionEvent->m_ReactionLOS == ReactionLOS_Visual ? 50.0f : 60.0f, 32, reactionEvent->m_ReactionLOS == ReactionLOS_Visual ? FColor::Green : FColor::Red, false, reactionEvent->m_ReactionLOS == ReactionLOS_Visual ? 3.0f : 0.5f);

    if (reactionEvent->m_ReactionLOS == ReactionLOS_Visual)
    {
        FVector diffToNpc = npcPosition - reactionEvent->m_TargetPosition;
        float distNpcSQ = diffToNpc.SizeSquared();

        float radiusSQ = 1500.0f*1500.0f;

        if (distNpcSQ < radiusSQ)
        {
            m_IsBlind = true;
            m_BlindTimeLeft = 3.0f;
        }
        else
        {
            m_IsLooking = true;
            m_LookPosition = reactionEvent->m_TargetPosition;
            m_LookTimeLeft = 1.25f;
        }
    }
}

void AAIBase::ProcessMovement(float DeltaTime)
{
    FVector actorLocation = GetActorLocation();
    
    //DrawDebugLine(GetWorld(), actorLocation, FVector::ZeroVector, FColor::Green, false);

    if (m_IsBlind && !ProcessBlindness(DeltaTime))
    {
        return;
    }
    else if (m_IsPlayerDetected && !ProcessPlayerDetection(DeltaTime))
    {
        return;
    }
    else if (m_CurrentDestination == FVector::ZeroVector)
    {
        m_CurrentDestination = m_NextDestination;
        SetNewMoveDestination(m_NextDestination);
    }
    
    if (HasReachedDestination(actorLocation))
    {
        UpdateNextDestination();
    }
}

void AAIBase::ProcessAvoidance()
{
    FVector npcPosition = GetActorLocation();
    FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
    UWorld* npcWorld = GetWorld();

    ObjectPartition* op = ObjectPartition::GetInstance();
    if (op)
    {
        TArray<AActor*> actors;
        op->GatherObjectsAroundPosition(this, npcPosition, 250.f, actors);
        if (actors.Num() > 0)
        {
            //DrawDebugSphere(npcWorld, npcHead, 20.0f, 32, FColor::Magenta);
        }
    }
}

void AAIBase::DetectPlayer()
{
    m_IsPlayerDetected = false;
    AActor* targetPlayer = NULL;
    
    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        targetPlayer = reactionManager->GetPlayer();
    }

    if (targetPlayer)
    {
        bool canSeePlayer = false;
        FVector npcPosition = GetActorLocation();
        FVector targetPosition = targetPlayer->GetActorLocation();
        FVector distToTarget = npcPosition - targetPosition;
        if (distToTarget.Size() < 1500.0f)
        {
            FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
            FVector playerHead = targetPosition + FVector::UpVector * 200.0f;
            FVector playerTorso = targetPosition + FVector::UpVector * 100.0f;
            FVector playerFeet = targetPosition + FVector::UpVector * 25.0f;

            UWorld* npcWorld = GetWorld();
            int bodyPartSeen = 0;

            if (!SDTUtils::Raycast(npcWorld, npcHead, playerHead))
            {
                //DrawDebugLine(npcWorld, npcHead, playerHead, FColor::Blue, false, 0.066f);
                ++bodyPartSeen;
            }
            if (!SDTUtils::Raycast(npcWorld, npcHead, playerTorso))
            {
                //DrawDebugLine(npcWorld, npcHead, playerTorso, FColor::Blue, false, 0.066f);
                ++bodyPartSeen;
            }
            if (!SDTUtils::Raycast(npcWorld, npcHead, playerFeet))
            {
                //DrawDebugLine(npcWorld, npcHead, playerFeet, FColor::Blue, false, 0.066f);
                ++bodyPartSeen;
            }

            if (bodyPartSeen > 1)
            {
               // m_IsPlayerDetected = true;
            }
        }
    }
}

bool AAIBase::HasReachedDestination(const FVector actorLocation)
{
    return (actorLocation - m_NextDestination).Size2D() < 50.f;
}

void AAIBase::UpdateNextDestination()
{
    if (m_PatrolPath)
    {
        if (m_PatrolPath->GetNWayPoints() > 0)
        {
            m_PatrolPath->GetNextWayPoint(m_CurrentWayPoint, m_NextWaypoint, m_PathDirection);
            if (m_NextWaypoint != nullptr)
            {
                if (AActor* waypointActor = m_NextWaypoint->GetOwner())
                {
                    m_NextDestination = waypointActor->GetActorLocation();
                    m_CurrentDestination = m_NextDestination;
                    SetNewMoveDestination(m_NextDestination);
                }
            }
        }
    }
}

void AAIBase::SetInitialPatrolling()
{
    PatrolPathManager* ppm = PatrolPathManager::GetInstance();
    if (ppm)
    {
        m_PatrolPath = ppm->GetRandomPath();
        if (m_PatrolPath && m_PatrolPath->GetNWayPoints() > 0)
        {
            m_CurrentWayPoint = rand() % m_PatrolPath->GetNWayPoints();
            m_PathDirection = 1;
        }
    }

    FVector actorLocation = GetActorLocation();
    m_NextDestination = actorLocation;
    m_CurrentDestination = actorLocation;
}

void AAIBase::SetMaxWalkingSpeed()
{
    GetCharacterMovement()->MaxWalkSpeed = (float)(rand() % 200 + 100);
}

