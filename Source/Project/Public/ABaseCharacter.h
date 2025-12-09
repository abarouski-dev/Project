#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat.h"
#include "ABaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, NewStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, int, NewHealth);

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


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health HP")
	int Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health HP")
	int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina SP")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina SP")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina SP")
	float StaminaRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina SP")
	float StaminaCostAttack;


	bool CanPayStaminaCost(float Cost);
	void PayStamina(float Cost);
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(int value) override;

	UPROPERTY(BlueprintAssignable, Category = "Stamina SP")
	FOnStaminaChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health HP")
	FOnHealthChanged OnHealthChanged;
};