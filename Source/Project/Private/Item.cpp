#include "Item.h"

AItem::AItem()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AItem::Interact_Implementation(AActor* Caller)
{
    PickUp(Caller);
}

void AItem::PickUp_Implementation(AActor* Caller)
{
    
}

