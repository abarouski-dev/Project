#include "Weapon.h"
#include "ABasePlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void AWeapon::PickUp_Implementation(AActor* Caller)
{
    if (!Caller) return;

    AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(Caller);
    if (Player)
    {
        Player->EquipWeapon(this);
    }
}
