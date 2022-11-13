#pragma once

#include "CoreMinimal.h"
#include "Animation/AIPatrol.h"
#include "Animation_Exercice2_Target.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API AAnimation_Exercice2_Target : public AAIPatrol
{
	GENERATED_BODY()

protected:
    virtual void UpdateNavigation_Exercice2(float DeltaTime, float currentSpeed, float targetSpeed, float& newSpeed) override;
    virtual void UpdateLayeredAction_Exercice2(UPatrolLayeredAction layeredAction) override;

    virtual int GetNoExercice() { return 2; }

    // Data exposed for speed
    UPROPERTY(Category = "Exercice", VisibleAnywhere, BlueprintReadOnly)
    float AnimationNavigationSpeed = 0.0f;

    UPROPERTY(Category = "Exercice", VisibleAnywhere, BlueprintReadOnly)
    bool IsWaving = false;
};
