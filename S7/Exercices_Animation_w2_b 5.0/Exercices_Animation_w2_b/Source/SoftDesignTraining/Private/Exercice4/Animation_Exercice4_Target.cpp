#pragma optimize("",off)
#include "Exercice4/Animation_Exercice4_Target.h"
#include "SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "SDTUtils.h"

void AAnimation_Exercice4_Target::ProcessLookAt_Exercice4(FVector npcPosition, FVector targetPosition, FTransform headTransformWorldSpace, float deltaTime, FTransform& outHeadTransform)
{
    float addPitch = 0.0f;
    float addYaw = 0.0f;

    FVector distToTarget = npcPosition - targetPosition;
    static float distThreshold = 500.0f;


    // Fixed vertical offset to prevent looking at the ground
    static float targetZOffset = 80.0f;
    targetPosition[2] += targetZOffset;

    // Acquire my location
    FVector originPosition = headTransformWorldSpace.GetTranslation();

    // Compute me to target direction
    FVector npc2target = targetPosition - originPosition;

    DrawDebugLine(GetWorld(), originPosition, targetPosition, FColor::Cyan, false);
    DrawDebugSphere(GetWorld(), targetPosition, 10.0f, 32, FColor::Cyan, false, -1.0f, 0);

    FTransform npcTransform = GetActorTransform();

    // Understand various axis systems involved by drawing them
    {
        static const FColor LookAt_X_MidA(192, 0, 0, 192);
        static const FColor LookAt_Y_MidA(0, 192, 0, 192);
        static const FColor LookAt_Z_MidA(0, 0, 192, 192);

        static const FColor LookAt_X_LowA(96, 0, 0, 96);
        static const FColor LookAt_Y_LowA(0, 96, 0, 96);
        static const FColor LookAt_Z_LowA(0, 0, 96, 96);

        FVector npcAxisX = npcTransform.GetUnitAxis(EAxis::X);
        FVector npcAxisY = npcTransform.GetUnitAxis(EAxis::Y);
        FVector npcAxisZ = npcTransform.GetUnitAxis(EAxis::Z);
        DrawDebugLine(GetWorld(), npcTransform.GetTranslation(), npcTransform.GetTranslation() + npcAxisX * 50.0f, LookAt_X_LowA, false);
        DrawDebugLine(GetWorld(), npcTransform.GetTranslation(), npcTransform.GetTranslation() + npcAxisY * 50.0f, LookAt_Y_LowA, false);
        DrawDebugLine(GetWorld(), npcTransform.GetTranslation(), npcTransform.GetTranslation() + npcAxisZ * 50.0f, LookAt_Z_LowA, false);

        FVector headAxisX = headTransformWorldSpace.GetUnitAxis(EAxis::X);
        FVector headAxisY = headTransformWorldSpace.GetUnitAxis(EAxis::Y);
        FVector headAxisZ = headTransformWorldSpace.GetUnitAxis(EAxis::Z);
        DrawDebugLine(GetWorld(), headTransformWorldSpace.GetTranslation(), headTransformWorldSpace.GetTranslation() + (headAxisX * 70.0f), LookAt_X_MidA, false);
        DrawDebugLine(GetWorld(), headTransformWorldSpace.GetTranslation(), headTransformWorldSpace.GetTranslation() + (headAxisY * 70.0f), LookAt_Y_MidA, false);
        DrawDebugLine(GetWorld(), headTransformWorldSpace.GetTranslation(), headTransformWorldSpace.GetTranslation() + (headAxisZ * 70.0f), LookAt_Z_MidA, false);
    }

    // Translate look-at world space Direction into NPC space
    FVector npc2targetComp = npcTransform.InverseTransformVector(npc2target);
    FRotator headDirectionCompRot = npc2targetComp.Rotation();

    // Write proper angle on proper bone axis
    addPitch = -headDirectionCompRot.Pitch;
    addYaw = -headDirectionCompRot.Yaw;
    //addRoll = headDirectionCompRot.Roll;

    // Dumb yaw clamping at 90 degrees edges
    static float yawThreshold = 90.0f;
    static float pitchThreshold = 60.0f;
    addYaw = FMath::ClampAngle(addYaw, -yawThreshold, yawThreshold);
    addPitch = FMath::ClampAngle(addPitch, -pitchThreshold, pitchThreshold);


    if (FMath::IsNearlyZero(addYaw) && FMath::IsNearlyZero(m_LookAtYawValue_T) && FMath::IsNearlyZero(addPitch) && FMath::IsNearlyZero(m_LookAtPitchValue_T))
    {
        m_LookAtYawValue_T = addYaw = 0.0f;
        m_LookAtPitchValue_T = addPitch = 0.0f;
        // Do nothing
    }
    else
    {
        static float yawDamp = 0.09f;
        static float pitchDamp = 0.03f;
        m_LookAtYawValue_T = SDTUtils::ExponentialDamp(m_LookAtYawValue_T, addYaw, yawDamp, deltaTime);
        m_LookAtPitchValue_T = SDTUtils::ExponentialDamp(m_LookAtPitchValue_T, addPitch, pitchDamp, deltaTime);

        // Build additive quaternion objects
        FRotator headDirectionCompRotYaw = FRotator::ZeroRotator;
        headDirectionCompRotYaw.Roll = m_LookAtYawValue_T;
        FQuat addRotYaw = headDirectionCompRotYaw.Quaternion();
        addRotYaw.Normalize();

        FRotator headDirectionCompRotPitch = FRotator::ZeroRotator;
        headDirectionCompRotPitch.Yaw = m_LookAtPitchValue_T;
        FQuat addRotPitch = headDirectionCompRotPitch.Quaternion();
        addRotPitch.Normalize();

        // Apply additive quaternion rotation
        FQuat newRot = m_rotationOffsetBetweenNeckAndHead_T * addRotYaw * addRotPitch;

        // Overwrite current head bone transform's rotation
        FTransform boneTransform;
        boneTransform.SetTranslation(outHeadTransform.GetTranslation());
        boneTransform.SetRotation(newRot);

        outHeadTransform.SetTranslation(boneTransform.GetTranslation());
        outHeadTransform.SetRotation(boneTransform.GetRotation());
    }
}