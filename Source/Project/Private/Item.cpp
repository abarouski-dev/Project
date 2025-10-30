#include "Item.h"

AItem::AItem()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AItem::PickUp_Implementation(AActor* Caller)
{
    PickUp(Caller);
}
