#include "InteractionComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::Interact()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector Start = Owner->GetActorLocation();
    FVector End = Start + Owner->GetActorForwardVector() * InteractionDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        if (Hit.GetActor() && Hit.GetActor()->Implements<UInteractionInterface>())
        {
            IInteractionInterface::Execute_Interact(Hit.GetActor(), Owner);
        }
    }

    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.f);
}