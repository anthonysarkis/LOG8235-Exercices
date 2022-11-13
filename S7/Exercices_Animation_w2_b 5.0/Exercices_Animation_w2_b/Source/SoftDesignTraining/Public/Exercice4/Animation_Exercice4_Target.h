#pragma once

#include "CoreMinimal.h"
#include "Exercice4/Animation_Exercice4_Base.h"
#include "Animation_Exercice4_Target.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API AAnimation_Exercice4_Target : public AAnimation_Exercice4_Base
{
	GENERATED_BODY()

protected:
    
    virtual void ProcessLookAt_Exercice4(FVector npcPosition, FVector targetPosition, FTransform headTransformWorldSpace, float deltaTime, FTransform& outHeadTransform);

// User Code
    float m_LookAtYawValue_T;
    float m_LookAtPitchValue_T;
};
