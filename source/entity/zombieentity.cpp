#include "zombieentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"
#include "../manager/guimanager.h"
#include <cmath>

ENTITY_CREATOR("Zombie", ZombieEntity)

ZombieEntity::ZombieEntity()
	: EnemyEntity("Zombie", "Zombie")
{
	fVelocity = 0.55f;
	sEnemy.displayName = "Zombie";
}

void ZombieEntity::Update(f32 dt)
{
	//if (!pBody)
		//return;

	if (fInvicibleTime > 0)
	{
		pSprite->SetVisible(!pSprite->IsVisible());

		fInvicibleTime -= dt;
		if (fInvicibleTime <= 0)
		{
			pSprite->SetVisible(true);
			fInvicibleTime = 0;
		}
	}

	// Search a nerby player
	if (pTarget == nullptr)
		pTarget = static_cast<Player1Entity *>(gWorldManager->FindEntityByClassName("Player1"));
	if (pTarget == nullptr)
		pTarget = static_cast<Player3Entity *>(gWorldManager->FindEntityByClassName("Player3"));
	if (pTarget == nullptr)
		pTarget = static_cast<Player4Entity *>(gWorldManager->FindEntityByClassName("Player4"));

	if (pTarget != nullptr)
	{
		auto dir = pTarget->GetBodyPosition() - pBody->GetPosition();
		auto distance = dir.Normalize();


		if (bPlayerLock)
		{
			if (distance > 2.0f)
			{
				Log("Perdeu o player");
				this->StopToFollow();
			}
			else
			{
				this->FindPathToPlayer();
			}
		}
		else
		{
			if (distance <= 2.0f)
			{
				Log("Achou o player");
				bPlayerLock = true;
			}
		}

	}
}
