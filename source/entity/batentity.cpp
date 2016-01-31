#include "batentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"
#include "../manager/guimanager.h"
#include <cmath>

ENTITY_CREATOR("Bat", BatEntity)

BatEntity::BatEntity()
	: EnemyEntity("Bat", "Bat")
{
	fVelocity = 0.55f;
	sEnemy.displayName = "Bat";
}

void BatEntity::Update(f32 dt)
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


	if (pTarget != nullptr)
	{
		auto dir = pTarget->GetBodyPosition() - pBody->GetPosition();
		auto distance = dir.Normalize();

		if (distance > 2.0f)
		{
			this->StopToFollow();
			pTarget = nullptr;
		}
		else if(distance <= 0.8)
		{
			if(pTarget->GetInvencibleTime() == 0)
				pTarget->OnDamage(b2Vec2(0, 0), 1);
		}
		else
		{
			this->FindPathToPlayer();
		}
	}
}

