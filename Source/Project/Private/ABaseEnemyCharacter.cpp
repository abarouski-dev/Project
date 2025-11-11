#include "ABaseEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AABaseEnemyCharacter::AABaseEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
    PawnSensingComp->SightRadius = 1500.f;
    PawnSensingComp->SetPeripheralVisionAngle(120.f);
}

void AABaseEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (PawnSensingComp)
    {
        PawnSensingComp->OnSeePawn.AddDynamic(this, &AABaseEnemyCharacter::OnSeePawn);
    }

    if (WeaponClass)
    {
        CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
        CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
        CurrentWeapon->SubscribeHit();
        CurrentWeapon->ownerCharacter = this;
        DisableWeaponCollision();
    }
}

void AABaseEnemyCharacter::OnSeePawn(APawn* Pawn)
{
    if (!Pawn) return;


    AAIController* AICon = Cast<AAIController>(GetController());

    float Distance = FVector::Dist(GetActorLocation(), Pawn->GetActorLocation());

    if (Distance > AttackRange && !bIsAttacking)
    {
        AICon->MoveToActor(Pawn, 5.0f);
    }
    else
    {
        AICon->StopMovement();

        if (AttackMontage && !bIsAttacking)
        {
            PlayAnimMontage(AttackMontage);
            bIsAttacking = true;
        }
    }
}

void AABaseEnemyCharacter::EnableWeaponCollision()
{
    if (CurrentWeapon)
        CurrentWeapon->EnableCollision();
    bIsAttacking = true;
}

void AABaseEnemyCharacter::DisableWeaponCollision()
{
    if (CurrentWeapon)
        CurrentWeapon->DisableCollision();
    bIsAttacking = false;
}

void AABaseEnemyCharacter::Hit()
{
    if (GetHitMontage)
    {
        PlayAnimMontage(GetHitMontage);
        if (MySound) {
            UGameplayStatics::PlaySoundAtLocation(this, MySound, GetActorLocation());
        }
    }
}

void AABaseEnemyCharacter::Death()
{
    UE_LOG(LogTemp, Warning, TEXT("Character is dead"));
    Destroy();
}