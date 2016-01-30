#include "player1entity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("Player1", Player1Entity)

Player1Entity::Player1Entity()
	: PlayerEntity("Player1", "Player1", false)
{
	fVelocity = 1.5f;

	sPlayer.iKey = 2;
	sPlayer.displayName = "Player1";
	sPlayer.iLevel = 1;
	sPlayer.iXP = 100;
	sPlayer.iAttackPower = 10;
	sPlayer.iDefensePower = 10;
	sPlayer.iGold = 0;
	sPlayer.iLife = 22;
	sPlayer.iLifeTotal = 22;
	sPlayer.iStamina = 10;
	sPlayer.iStaminaTotal = 10;
}
