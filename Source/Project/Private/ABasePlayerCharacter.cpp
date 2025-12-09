#include "ABasePlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionInterface.h"   
#include "Components/InputComponent.h"
#include "InteractionComponent.h" 
#include <Kismet/GameplayStatics.h>
#include "Weapon.h"

AABasePlayerCharacter::AABasePlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void AABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
}

void AABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Look);
		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Interact);
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Attack);
	}
}

void AABasePlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();
	if (Controller != nullptr && (CharacterState == ECharacterState::Idle || CharacterState == ECharacterState::OutOfStamina))
	{
		GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);

		if (CharacterState == ECharacterState::OutOfStamina)
			Movement *= 0.5f;

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, Movement.Y);
		AddMovementInput(RightDir, Movement.X);
	}
}

void AABasePlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(-LookAxis.Y);
}

void AABasePlayerCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractionComponent)
	{
		InteractionComponent->Interact();
	}
}

void AABasePlayerCharacter::EquipWeapon(AWeapon* Weapon)
{
	if (!Weapon) return;

	if (Weapon->IsEquiped()) 
	{
		return;
	}

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (CharacterMesh)
	{
		FName SocketName("WeaponSocket");
		Weapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Weapon->GetRootComponent()))
		{
			PrimComp->SetSimulatePhysics(false);
			PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	CurrentWeapon = Weapon;
	CurrentWeapon->ownerCharacter = this;
	CurrentWeapon->DisableCollision();
}

void AABasePlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (!CurrentWeapon || GetCharacterMovement()->IsFalling() || CharacterState != ECharacterState::Idle || !AttackMontage) return;
	if (!CanPayStaminaCost(StaminaCostAttack))
	{
		CharacterState = ECharacterState::OutOfStamina;

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				CharacterState = ECharacterState::Idle;
			}, 1, false);
		return;
	}

	PayStamina(StaminaCostAttack);
	CharacterState = ECharacterState::Attacking;
	PlayAnimMontage(AttackMontage);
}

void AABasePlayerCharacter::GetHit_Implementation(int value)
{
	if (GetHitMontage)
	{
		PlayAnimMontage(GetHitMontage);
		if (CurrentWeapon)	CurrentWeapon->DisableCollision();
		CharacterState = ECharacterState::GettingHit;
	}
	if (MySound) {
		UGameplayStatics::PlaySoundAtLocation(this, MySound, GetActorLocation());
	}
	Super::GetHit_Implementation(value);
}


void AABasePlayerCharacter::EnableWeaponCollision()
{
	if (CurrentWeapon)
		CurrentWeapon->EnableCollision();
}

void AABasePlayerCharacter::DisableWeaponCollision()
{
	if (CurrentWeapon)
		CurrentWeapon->DisableCollision();
	if (CharacterState != ECharacterState::Dead)
		CharacterState = ECharacterState::Idle;
	bIsAttacking = false;
}

void AABasePlayerCharacter::Death_Implementation()
{
	CharacterState = ECharacterState::Dead;
	UE_LOG(LogTemp, Warning, TEXT("Player is dead"));

	if (CurrentWeapon)
		CurrentWeapon->Destroy();
	Destroy();

}