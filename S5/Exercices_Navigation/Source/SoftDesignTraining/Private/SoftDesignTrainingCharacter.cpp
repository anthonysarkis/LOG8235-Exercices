// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SoftDesignTrainingCharacter.h"
#include "SoftDesignTraining.h"
#include "SDTNavigation/DesignTrainingMovementComponent.h"
#include "ReactionManager.h"
#include "DrawDebugHelpers.h"
#include "SDTNavigation/SDT_WorldSettings.h"
#include "Components/SceneComponent.h"


ASoftDesignTrainingCharacter::ASoftDesignTrainingCharacter(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer.SetDefaultSubobjectClass<UDesignTrainingMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	//USceneComponent::SetupAttachment();
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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// Create a camera boom...
	FAttachmentTransformRules rules(EAttachmentRule::KeepRelative, false);
	CameraBoom->AttachToComponent(RootComponent, rules);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 2000.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachToComponent(CameraBoom, rules, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ASoftDesignTrainingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector currentLocation = GetActorLocation();

    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        UWorld* currentWorld = GetWorld();

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

void ASoftDesignTrainingCharacter::PlaceBomb()
{
	ReactionManager* reactionManager = ReactionManager::GetInstance();
	if (reactionManager)
	{
		reactionManager->CreateReactionEvent(GetActorLocation(), 6250000.0f, ReactionType_Boom, ReactionLOS_Sound);
		reactionManager->CreateReactionEvent(GetActorLocation(), 6250000.0f, ReactionType_Boom, ReactionLOS_Visual);
	}
}
