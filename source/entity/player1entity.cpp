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
	this->SetIsActive(true);
}

bool Player1Entity::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	if (this->bIsActive && this->bIsInputEnabled)
	{
		Key k = ev->GetKey();

//		b2Vec2 vel = pBody->GetLinearVelocity();

		if (k == eKey::Up)
		{
			SetState(Runing);
			fUpDownMove = -1;
		}

		if (k == eKey::Left)
		{
			SetState(Runing);
			fMove = -1;
		}

		if (k == eKey::Right)
		{
			SetState(Runing);
			fMove = 1;
		}

		if (k == eKey::Down)
		{
			SetState(Runing);
			fUpDownMove = 1;
		}

		if (k == eKey::Enter)
		{
			SetState(Attacking);
			Attack();
		}
	}

	return true;
}

bool Player1Entity::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	if (this->bIsActive && this->bIsInputEnabled)
	{
		Key k = ev->GetKey();

		b2Vec2 vel = pBody->GetLinearVelocity();
		vel.x = 0;
		vel.y = 0;

		// Remove the directions
		if (k == eKey::Up|| k == eKey::W)
		{
			pBody->SetLinearVelocity(vel);
			fUpDownMove = 0;
		}

		if (k == eKey::Left|| k == eKey::A)
		{
			pBody->SetLinearVelocity(vel);
			fMove = 0;
		}

		if (k == eKey::Right|| k == eKey::D)
		{
			pBody->SetLinearVelocity(vel);
			fMove = 0;
		}

		if (k == eKey::Down|| k == eKey::S)
		{
			pBody->SetLinearVelocity(vel);
			fUpDownMove = 0;
		}

		if (fUpDownMove == 0 && fMove == 0)
		{
			SetState(Waiting);
		}
	}

	return true;
}

