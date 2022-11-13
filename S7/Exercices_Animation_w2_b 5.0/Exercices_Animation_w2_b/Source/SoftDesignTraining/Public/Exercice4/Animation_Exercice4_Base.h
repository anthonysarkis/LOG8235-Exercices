#pragma once

#include "GameFramework/Character.h"
#include "AIBase.h"
#include "CoreMinimal.h"
#include "Animation_Exercice4_Base.generated.h"

UCLASS(Blueprintable)
class SOFTDESIGNTRAINING_API AAnimation_Exercice4_Base : public AAIBase
{
    GENERATED_BODY()

protected:
    AAnimation_Exercice4_Base();

    const FName m_HeadName = TEXT("Head");

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exercice4")
    FRotator HeadRotator;
};
