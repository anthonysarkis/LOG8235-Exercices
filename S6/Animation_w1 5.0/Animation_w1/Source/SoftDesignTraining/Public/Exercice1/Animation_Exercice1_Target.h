#pragma once

#include "CoreMinimal.h"
#include "Animation/AIPatrol.h"
#include "Animation_Exercice1_Target.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API AAnimation_Exercice1_Target : public AAIPatrol
{
	GENERATED_BODY()

protected:
    virtual void UpdateNavigation_Exercice1(float targetSpeed, float dt) override;
    virtual int GetNoExercice() { return 1; }

    
    UPROPERTY(Category = "Exercice", VisibleAnywhere, BlueprintReadOnly)
    float AnimationNavigationSpeed = 0.0f;
};