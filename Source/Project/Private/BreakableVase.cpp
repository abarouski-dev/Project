#include "BreakableVase.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"

ABreakableVase::ABreakableVase()
{
    VaseMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("VaseMesh"));
    RootComponent = VaseMesh;

    VaseMesh->SetSimulatePhysics(true);
    VaseMesh->SetNotifyRigidBodyCollision(true);
    VaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    VaseMesh->SetGenerateOverlapEvents(true);
}

void ABreakableVase::ApplyHit(const FHitResult& Hit)
{
    if (!VaseMesh) return;

    FVector ImpulseDirection = Hit.ImpactNormal * -1.0f;
    VaseMesh->AddImpulseAtLocation(ImpulseDirection * 0.1f, Hit.ImpactPoint, Hit.BoneName);

    UE_LOG(LogTemp, Warning, TEXT("Vase hit at: %s"), *Hit.ImpactPoint.ToString());
}
