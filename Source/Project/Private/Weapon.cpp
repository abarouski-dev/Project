#include "Weapon.h"
#include "ABasePlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "Combat.h"

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

    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
    BoxTraceStart->SetupAttachment(GetRootComponent());

    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
    BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::EnableCollision()
{
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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

void AWeapon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other,
    UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
    FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Error, TEXT("Hit actor: %s"), *Other->GetName());
}

void AWeapon::OnOverlapStart(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Error, TEXT("Overlapped actor: %s | %s"), *OtherActor->GetName(), *ownerCharacter->GetName());

    if (ownerCharacter && ownerCharacter == OtherActor) return;

    FHitResult BoxHit;
    BoxTrace(BoxHit);
    CreateForceFields(BoxHit.ImpactPoint);

    if (OtherActor->Implements<UCombat>())
    {
        ICombat::Execute_GetHit(OtherActor, Damage);
        DisableCollision();
    }
}

void AWeapon::BoxTrace(FHitResult& BoxHit) {
    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(this);
    IgnoredActors.Add(GetOwner());
    for (AActor* Actor : IgnoreActors) {
        IgnoredActors.Add(Actor);
    }

    UKismetSystemLibrary::BoxTraceSingle(
        GetWorld(),
        BoxTraceStart->GetComponentLocation(),
        BoxTraceEnd->GetComponentLocation(),
        WeaponCollision->GetScaledBoxExtent(),
        BoxTraceStart->GetComponentRotation(),
        UEngineTypes::ConvertToTraceType(ECC_Pawn),
        false,
        IgnoredActors,
        EDrawDebugTrace::ForOneFrame,
        BoxHit,
        true
    );
    IgnoreActors.AddUnique(BoxHit.GetActor());
}

bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
    return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

bool AWeapon::IsEquiped()
{
    if (!bIsEquiped) 
    {
        bIsEquiped = true;
        return false;
    }    
	return true;
}

void AWeapon::ExecuteGetHit(FHitResult& OtherActor)
{
    ICombat* CombatInterface = Cast<ICombat>(OtherActor.GetActor());
    if (CombatInterface)
    {
        CombatInterface->Execute_GetHit(OtherActor.GetActor(), Damage);
    }
}