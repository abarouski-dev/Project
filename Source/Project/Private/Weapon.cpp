#include "Weapon.h"
#include "ABasePlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"

void AWeapon::PickUp_Implementation(AActor* Caller)
{
    if (!Caller) return;

    AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(Caller);
    if (Player && !Player->CurrentWeapon)
    {
        Player->EquipWeapon(this);
        SubscribeHit();
    }

}

AWeapon::AWeapon()
{
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;

    WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
    WeaponCollision->SetupAttachment(WeaponMesh);
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    WeaponCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void AWeapon::EnableCollision()
{
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    WeaponCollision->SetHiddenInGame(false);
    WeaponCollision->SetVisibility(true);
}

void AWeapon::DisableCollision()
{
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponCollision->SetHiddenInGame(true);
    WeaponCollision->SetVisibility(false);
}

void AWeapon::SubscribeHit()
{
    if (!WeaponCollision->OnComponentBeginOverlap.IsAlreadyBound(this, &AWeapon::OnOverlapStart))
    {
        WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapStart);
    }
}

void AWeapon::OnOverlapStart(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{

    if (ownerCharacter && ownerCharacter == OtherActor) return;


    if (OtherActor->Implements<UCombat>())
    {
        ICombat::Execute_GetHit(OtherActor, 50);
		DisableCollision();
    }
}
