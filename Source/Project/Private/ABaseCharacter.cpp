#include "ABaseCharacter.h"

AABaseCharacter::AABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}


void AABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABaseCharacter::GetHit_Implementation(int value)
{
	Health -= value;
	if (value > 0) {
		Hit();
	}
	if (Health > MaxHealth) {
		Health = MaxHealth;
	}
	if (Health <= 0)
	{
		Death();
	}
}

void AABaseCharacter::Death()
{
}

void AABaseCharacter::Hit()
{
}

void AABaseCharacter::EnableWeaponCollision()
{
}

void AABaseCharacter::DisableWeaponCollision()
{
}