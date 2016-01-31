#include "wolfentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"
#include "../manager/guimanager.h"
#include <cmath>

ENTITY_CREATOR("Wolf", WolfEntity)

WolfEntity::WolfEntity()
	: EnemyEntity("Wolf", "Wolf")
{
	fVelocity = 0.55f;
	sEnemy.displayName = "Wolf";
}

void WolfEntity::Update(f32 dt)
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
		pTarget = static_cast<Player2Entity *>(gWorldManager->FindEntityByClassName("Player2"));
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
