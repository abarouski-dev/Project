#include "ABaseCharacter.h"

AABaseCharacter::AABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}