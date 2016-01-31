#include "player3entity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("Player3", Player3Entity)

Player3Entity::Player3Entity()
	: PlayerEntity("Player3", "Player3", false)
{
	fVelocity = 1.5f;
	sPlayer.iKey = 3;
	sPlayer.displayName = "Player3";
	sPlayer.iLevel = 1;
	sPlayer.iXP = 200;
	sPlayer.iAttackPower = 5;
	sPlayer.iDefensePower = 15;
	sPlayer.iGold = 0;
	sPlayer.iLife = 3;
	sPlayer.iLifeTotal = 12;
	sPlayer.iStamina = 20;
	sPlayer.iStaminaTotal = 20;
}
