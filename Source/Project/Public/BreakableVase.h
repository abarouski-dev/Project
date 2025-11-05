#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "BreakableVase.generated.h"

UCLASS()
class PROJECT_API ABreakableVase : public AActor
{
    GENERATED_BODY()

public:
    ABreakableVase();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UGeometryCollectionComponent* VaseMesh;

    void ApplyHit(const FHitResult& Hit);
};

