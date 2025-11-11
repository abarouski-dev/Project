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

	UFUNCTION() void OnOverlapStart(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResu1t);

public:
	class AABaseCharacter* ownerCharacter;
	AWeapon();
	virtual void PickUp_Implementation(AActor* Caller) override;
	void EnableCollision();
	void DisableCollision();
	void SubscribeHit();
};
