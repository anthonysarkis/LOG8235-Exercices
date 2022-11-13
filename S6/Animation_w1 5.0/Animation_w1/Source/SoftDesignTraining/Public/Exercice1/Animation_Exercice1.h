#pragma once

#include "CoreMinimal.h"
#include "Animation/AIPatrol.h"
#include "Animation_Exercice1.generated.h"

// Exercice 1
//
// 1. (In Code) Create the properties to share data to the animations blueprint
// 2. (In Blueprint) Create the animation logic to manage the actors ground animations

#define WALK_ANIMATION_SPEED 200.0f
#define RUN_ANIMATION_SPEED 500.0f

UCLASS()
class SOFTDESIGNTRAINING_API AAnimation_Exercice1 : public AAIPatrol
{
	GENERATED_BODY()

protected:
    virtual void UpdateNavigation_Exercice1(float DeltaTime, float targetSpeed) override;
    virtual int GetNoExercice() { return 1; }
};
