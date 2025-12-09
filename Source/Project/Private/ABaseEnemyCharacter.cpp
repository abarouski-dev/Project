#include "ABaseEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

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
        if (CurrentWeapon->IsEquiped())
        {
            return;
        }
    }

    AAIController* AI = Cast<AAIController>(GetController());


    if (AI)
    {
        UBlackboardComponent* BB = AI->GetBlackboardComponent();
        if (BB)
        {
            BB->SetValueAsObject("ChassingTarget", TargetActor);
        }
    }
}

void AABaseEnemyCharacter::OnSeePawn(APawn* Pawn)
{
    UE_LOG(LogTemp, Warning, TEXT("AI %s!, x - %f | y - %f | %f"), *Pawn->GetName(), Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y, FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation()))

        float Distance = FVector::Dist(GetActorLocation(), Pawn->GetActorLocation());
    TargetActor->TeleportTo(Pawn->GetActorLocation(), FRotator::ZeroRotator);

    if (Distance < AttackRange && !bIsAttacking)
        CharacterState = EEnamyState::Attacking;

    else {
        CharacterState = EEnamyState::Chasing;
    }

    AAIController* AI = Cast<AAIController>(GetController());

    if (AI)
    {
        UBlackboardComponent* BB = AI->GetBlackboardComponent();
        if (BB)
        {
            BB->SetValueAsEnum("EnemyState", (uint8)CharacterState);
            BB->SetValueAsObject("CombatTarget", Pawn);
        }
    }
}

void AABaseEnemyCharacter::TryAttack()
{
    if (AttackMontage && !bIsAttacking)
    {
        PlayAnimMontage(AttackMontage);
        bIsAttacking = true;
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

    CharacterState = EEnamyState::Chasing;

    AAIController* AI = Cast<AAIController>(GetController());

    if (AI)
    {
        UBlackboardComponent* BB = AI->GetBlackboardComponent();
        if (BB)
        {
            BB->SetValueAsEnum("EnemyState", (uint8)CharacterState);
        }
    }
}

void AABaseEnemyCharacter::GetHit_Implementation(int value) {
    Super::GetHit_Implementation(value);
    if (GetHitMontage) {
        PlayAnimMontage(GetHitMontage);
        CurrentWeapon->DisableCollision();
    }
    if (MySound) {
        UGameplayStatics::PlaySoundAtLocation(this, MySound, GetActorLocation());
    }
}

void AABaseEnemyCharacter::Death_Implementation() 
{
    if (CurrentWeapon)
        CurrentWeapon->Destroy();
    Destroy();
}