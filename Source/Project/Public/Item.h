#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Item.generated.h"

UCLASS()
class PROJECT_API AItem : public AActor, public IInteractionInterface
{
    GENERATED_BODY()

public:
    AItem();

    virtual void Interact_Implementation(AActor* Caller) override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void PickUp(AActor* Caller);
    virtual void PickUp_Implementation(AActor* Caller);
};

