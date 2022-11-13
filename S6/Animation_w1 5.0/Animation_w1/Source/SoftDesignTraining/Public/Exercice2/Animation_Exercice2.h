#pragma once

#include "CoreMinimal.h"
#include "Animation/AIPatrol.h"
#include "Animation_Exercice2.generated.h"


// Exercice 2
//
// 1. (In Code) Add & tweak logic to damp the speed of the actor
// 2. (In Blueprint) Improve the blend logic of the animations in the graph & handle ai speed that differ from the matching animation speed

UCLASS()
class SOFTDESIGNTRAINING_API AAnimation_Exercice2 : public AAIPatrol
{
	GENERATED_BODY()

protected:

public:
    virtual void UpdateNavigation_Exercice2(float DeltaTime, float currentSpeed, float targetSpeed, float& outNewSpeed) override;
    virtual void UpdateLayeredAction_Exercice2(UPatrolLayeredAction layeredAction) override;

    virtual int GetNoExercice() { return 2; }

    // Data exposed for speed
    UPROPERTY(Category = "Exercice", VisibleAnywhere, BlueprintReadOnly)
    float AnimationNavigationSpeed = 0.0f;
};
