// Fill out your copyright notice in the Description page of Project Settings.


#include "DisableWeaponCollisionNotify.h"
#include "ABasePlayerCharacter.h"

void UDisableWeaponCollisionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->DisableWeaponCollision();
	}
}
