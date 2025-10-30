#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

void AWeapon::PickUp_Implementation(AActor* Caller)
{
    if (!Caller) return;

    ACharacter* Character = Cast<ACharacter>(Caller);
    if (Character)
    {
        USkeletalMeshComponent* Mesh = Character->GetMesh();
        if (Mesh)
        {
            FName SocketName("WeaponSocket");
            AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
        }
    }
}
