#pragma once

#include "CoreMinimal.h"
#include "Animation/AIPatrol.h"
#include "Animation_Exercice3.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API AAnimation_Exercice3 : public AAIPatrol
{
	GENERATED_BODY()

    virtual void UpdateNavigation_Exercice3(float DeltaTime, float currentSpeed, float targetSpeed, float& outNewSpeed, FVector currentPosition, FVector stopPosition) override;
    virtual int GetNoExercice() { return 3; }
protected:

    UPROPERTY(Category = "Exercice", VisibleAnywhere, BlueprintReadOnly)
    float AnimationNavigationSpeed = 0.0f;

    void OnTurnCompleted();
};
