// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

class SOFTDESIGNTRAINING_API SDTUtils
{
public:
    static bool Raycast(UWorld* uWorld, FVector sourcePoint, FVector targetPoint);

    static float ExponentialDamp(const float& val, const float& target, float damping, float dt)
    {
        return (((val * damping) + (target * dt)) / (damping + dt));
    }

    static FVector ExponentialDamp(const FVector& val, const FVector& target, float damping, float dt)
    {
        return (((val * damping) + (target * dt)) / (damping + dt));
    }

    static FQuat ExponentialDamp(const FQuat& val, const FQuat& target, float damping, float dt)
    {
        FQuat ret = FQuat::Slerp(val, target, dt / (damping + dt));
        return ret;
    }
};
