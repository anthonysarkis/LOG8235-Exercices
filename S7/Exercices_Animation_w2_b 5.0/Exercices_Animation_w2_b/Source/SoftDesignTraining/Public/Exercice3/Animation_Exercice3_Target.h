#pragma once

#include "CoreMinimal.h"
#include "Animation/AIPatrol.h"
#include "Animation_Exercice3_Target.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API AAnimation_Exercice3_Target : public AAIPatrol
{
	GENERATED_BODY()

protected:
    
    virtual void UpdateNavigation_Exercice3(float DeltaTime, float currentSpeed, float targetSpeed, float& outNewSpeed, FVector currentPosition, FVector stopPosition) override;
    virtual int GetNoExercice() { return 3; }

    // Data exposed for speed
    UPROPERTY(Category = "Exercice", VisibleAnywhere, BlueprintReadOnly)
    float AnimationNavigationSpeed = 0.0f;

    UPROPERTY(Category = "Exercice", VisibleAnywhere, BlueprintReadOnly)
    bool AnimationTurnAction = false;

    UFUNCTION(Category = "Exercice", BlueprintCallable)
    void OnTurnCompleted();

    // Stop Speed 
    bool ApproachStarted = false;
};
