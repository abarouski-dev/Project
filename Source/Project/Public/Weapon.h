#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECT_API AWeapon : public AItem
{
    GENERATED_BODY()

public:
    virtual void PickUp_Implementation(AActor* Caller) override;
};
