#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include <Perception/PawnSensingComponent.h>
#include "Weapon.h"
#include <Components/AudioComponent.h>
#include "PownStateEnum.h"
#include "ABaseEnemyCharacter.generated.h"

UCLASS()
class PROJECT_API AABaseEnemyCharacter : public AABaseCharacter
{
    GENERATED_BODY()

public:
    AABaseEnemyCharacter();

    virtual void EnableWeaponCollision() override;

    virtual void DisableWeaponCollision() override;

    UFUNCTION(BlueprintCallable)
    void TryAttack();

protected:
    bool bIsAttacking = false;
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UPawnSensingComponent* PawnSensingComp;

    UFUNCTION()
    void OnSeePawn(APawn* Pawn);


    virtual void GetHit_Implementation(int value) override;

    virtual void Death_Implementation() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float AttackRange = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    UAnimMontage* GetHitMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeapon> WeaponClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* MySound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    EEnamyState CharacterState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    AActor* TargetActor;
};
