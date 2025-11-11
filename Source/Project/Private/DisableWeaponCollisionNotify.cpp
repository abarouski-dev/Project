#include "DisableWeaponCollisionNotify.h"
#include "ABasePlayerCharacter.h"
#include "ABaseEnemyCharacter.h"

void UDisableWeaponCollisionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AABaseCharacter* Player = Cast<AABaseCharacter>(MeshComp->GetOwner()))
	{
		Player->DisableWeaponCollision();
	}
	else if (AABaseEnemyCharacter* Enemy = Cast<AABaseEnemyCharacter>(MeshComp->GetOwner()))
	{
		Enemy->DisableWeaponCollision();
	}
}
