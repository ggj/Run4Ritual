#include "player4entity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("Player4", Player4Entity)

Player4Entity::Player4Entity()
	: PlayerEntity("Player4", "Player4", true)
{
	fVelocity = 2.5f;

	sPlayer.iKey = 1;
	sPlayer.displayName = "Player4";
	sPlayer.iLevel = 1;
	sPlayer.iXP = 80;
	sPlayer.iAttackPower = 15;
	sPlayer.iDefensePower = 5;
	sPlayer.iGold = 10;
	sPlayer.iLife = 22;
	sPlayer.iLifeTotal = 22;
	sPlayer.iStamina = 5;
	sPlayer.iStaminaTotal = 5;
}
