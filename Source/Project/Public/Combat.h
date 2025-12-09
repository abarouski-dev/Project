#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Combat.generated.h"

UINTERFACE(MinimalAPI)
class UCombat : public UInterface
{
    GENERATED_BODY()
};

class ICombat
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void GetHit(int value);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Death();
};
