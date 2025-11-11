#include "EnableWeaponCollisionNotify.h"
#include "ABasePlayerCharacter.h"
#include "ABaseEnemyCharacter.h"

void UEnableWeaponCollisionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->EnableWeaponCollision();
	}
	else if (AABaseEnemyCharacter* Enemy = Cast<AABaseEnemyCharacter>(MeshComp->GetOwner()))
	{
		Enemy->EnableWeaponCollision();
	}
}
