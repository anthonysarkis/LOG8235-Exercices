// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SoftDesignTrainingCharacter.h"
#include "SoftDesignTraining.h"
#include "DesignTrainingMovementComponent.h"
//#include "Engine/Public/DrawDebugHelpers.h"
#include "ReactionManager.h"
#include "DrawDebugHelpers.h"
#include "SDT_WorldSettings.h"


ASoftDesignTrainingCharacter::ASoftDesignTrainingCharacter(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer.SetDefaultSubobjectClass<UDesignTrainingMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    //bomb position
    m_placedBombPosition = FVector::ZeroVector;
    m_bombExplosionPosition = FVector::ZeroVector;
    m_bombExplosionCooldown = 0.f;
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 2000.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ASoftDesignTrainingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector currentLocation = GetActorLocation();

    UWorld* currentWorld = GetWorld();
    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {

        int npcCount = reactionManager->m_NPCList.Num();

        for (int i = 0; i < npcCount; ++i)
        {
            AActor* npcCharacter = reactionManager->m_NPCList[i];
            if (npcCharacter)
            {
                DrawDebugLine(currentWorld, currentLocation, npcCharacter->GetActorLocation(), FColor::Red, false);
            }
        }
    }

    if (m_placedBombPosition != FVector::ZeroVector)
    {
        DrawDebugSphere(currentWorld, m_placedBombPosition, 60.f, 360, FColor::Orange);
    }
    else if (m_bombExplosionCooldown > 0.f)
    {
        m_bombExplosionCooldown -= DeltaTime;

        DrawDebugSphere(currentWorld, m_bombExplosionPosition, 500.f, 360, FColor::Red);

    }

    // TODO AF1: do spawning somewhere else. (maybe have a spawning manager object.)
    ASDT_WorldSettings * worldSettings = (ASDT_WorldSettings*) GetWorld()->GetWorldSettings();
    worldSettings->UpdateSpawn(GetWorld(), this);
}

void ASoftDesignTrainingCharacter::BeginPlay()
{
    Super::BeginPlay();

    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        reactionManager->RegisterPlayer(this);
    }
}

void ASoftDesignTrainingCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ReactionManager* reactionManager = ReactionManager::GetInstance();
	if (reactionManager)
	{
        reactionManager->UnregisterPlayer(this);
		reactionManager->Destroy();
	}
}

void ASoftDesignTrainingCharacter::BlindAgent()
{
    if (ReactionManager* reactionManager = ReactionManager::GetInstance())
    {
        reactionManager->CreateReactionEvent(GetActorLocation(), 6250000.0f, ReactionType_Blind, ReactionLOS_Visual);
    }
}

void ASoftDesignTrainingCharacter::PlaceBomb()
{
	ReactionManager* reactionManager = ReactionManager::GetInstance();
	if (reactionManager && m_bombExplosionCooldown <= 0.f && m_placedBombPosition == FVector::ZeroVector)
	{
		m_placedBombPosition = GetActorLocation();
		reactionManager->CreateReactionEvent(GetActorLocation(), 6250000.0f, ReactionType_Explosion, ReactionLOS_Visual);
		reactionManager->CreateReactionEvent(m_placedBombPosition, 6250000.0f, ReactionType_PlacedBomb, ReactionLOS_Sound);
	}
}

void ASoftDesignTrainingCharacter::ExplodeBomb()
{
	if (ReactionManager* reactionManager = ReactionManager::GetInstance())
	{
		if (m_placedBombPosition != FVector::ZeroVector)
		{
			m_bombExplosionCooldown = 3.f;
			m_bombExplosionPosition = m_placedBombPosition - FVector(0.f, 0.f, 50.f);
			m_placedBombPosition = FVector::ZeroVector;
			reactionManager->CreateReactionEvent(m_bombExplosionPosition, 6250000.0f, ReactionType_Explosion, ReactionLOS_Sound);
		}
	}
}

void ASoftDesignTrainingCharacter::ForceAllAgentsFlee()
{
    if (ReactionManager* reactionManager = ReactionManager::GetInstance())
    {
        reactionManager->CreateReactionEvent(GetActorLocation(), 6250000.0f, ReactionType_ForceFlee, ReactionLOS_Sound);
    }
}
