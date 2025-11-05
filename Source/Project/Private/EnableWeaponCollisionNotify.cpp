// Fill out your copyright notice in the Description page of Project Settings.


#include "EnableWeaponCollisionNotify.h"
#include "ABasePlayerCharacter.h"

void UEnableWeaponCollisionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->EnableWeaponCollision();
	}
}
