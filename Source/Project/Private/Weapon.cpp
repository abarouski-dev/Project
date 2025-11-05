#include "Weapon.h"
#include "ABasePlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

void AWeapon::PickUp_Implementation(AActor* Caller)
{
    if (!Caller) return;

    AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(Caller);
    if (Player)
    {
        Player->EquipWeapon(this);
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
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DisableCollision()
{
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}   

void AWeapon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other,
    UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
    FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Log, TEXT("Hit actor: %s"), *Other->GetName());
}
