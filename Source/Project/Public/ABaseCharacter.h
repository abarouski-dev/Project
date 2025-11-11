#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat.h"
#include "ABaseCharacter.generated.h"

UCLASS()
class PROJECT_API AABaseCharacter : public ACharacter, public ICombat
{
	GENERATED_BODY()

public:
	AABaseCharacter();


	virtual void EnableWeaponCollision();

	virtual void DisableWeaponCollision();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int MaxHealth;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(int value) override;
	virtual void Death();
	virtual void Hit();
};