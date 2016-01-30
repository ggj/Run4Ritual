#include "player2entity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("Player2", Player2Entity)

Player2Entity::Player2Entity()
	: PlayerEntity("Player2", "Player2", true)
{
	fVelocity = 2.5f;

	sPlayer.iKey = 1;
	sPlayer.displayName = "Player2";
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
