#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ABaseCharacter.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UBoxComponent* WeaponCollision;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other,
		UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
		FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION() void OnOverlapStart(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResu1t);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int Damage = 50;

	void BoxTrace(FHitResult& BoxHit);

	USceneComponent* BoxTraceStart;
	USceneComponent* BoxTraceEnd;

	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& OtherActor);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateForceFields(const FVector& ImpactPoint);

	bool bIsEquiped;

public:
	bool IsEquiped();
	class AABaseCharacter* ownerCharacter;
	TArray<AActor*> IgnoreActors;
	AWeapon();
	virtual void PickUp_Implementation(AActor* Caller) override;
	void EnableCollision();
	void DisableCollision();
	void SubscribeHit();
};