#include "amuletentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("Amulet", AmuletEntity)

AmuletEntity::AmuletEntity()
	: ItemEntity("Amulet", "Amulet")
{
}

void AmuletEntity::OnCollision(const CollisionEvent &event)
{
	if (event.GetType() == CollisionEventType::OnEnter)
	{
		Log("On collided with health potion");

		Entity *other = event.GetOtherEntity();
		if ((other != nullptr && other->GetClassName() == "Player1") ||
			(other != nullptr && other->GetClassName() == "Player2") ||
			(other != nullptr && other->GetClassName() == "Player3") ||
			(other != nullptr && other->GetClassName() == "Player4"))
		{
			Player1Entity *player = static_cast<Player1Entity *>(other);

			// Disable item
			this->pSprite->SetVisible(false);
			this->clSensor.Disable();

			//Collect Item
			player->OnCollect(ItemTypes::Amulet, this->iAmount);

		}
	}
}

