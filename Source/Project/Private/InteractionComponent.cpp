#include "InteractionComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "CollisionQueryParams.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::Interact()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector Start = CameraLocation;
    FVector Forward = CameraRotation.Vector();

    float SphereRadius = 120.f;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    if (AActor* Parent = Owner->GetAttachParentActor())
    {
        Params.AddIgnoredActor(Parent);
    }

    int32 Steps = 10;

    for (int32 i = 1; i <= Steps; i++)
    {
        FVector Center = Start + Forward * (InteractionDistance * i / Steps);
        TArray<FOverlapResult> Overlaps;

        bool bHit = GetWorld()->OverlapMultiByChannel(
            Overlaps,
            Center,
            FQuat::Identity,
            ECC_Visibility,
            FCollisionShape::MakeSphere(SphereRadius),
            Params
        );

        DrawDebugSphere(GetWorld(), Center, SphereRadius, 16, FColor::Green, false, 1.5f);

        for (auto& Overlap : Overlaps)
        {
            AActor* Actor = Overlap.GetActor();
            if (Actor && Actor->Implements<UInteractionInterface>())
            {
                IInteractionInterface::Execute_Interact(Actor, Owner);
                return;
            }
        }
    }
}