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
	sPlayer.iLife = 12;
	sPlayer.iLifeTotal = 12;
	sPlayer.iStamina = 20;
	sPlayer.iStaminaTotal = 20;
	this->SetIsActive(true);
}

bool Player3Entity::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	if (this->bIsActive && this->bIsInputEnabled)
	{
		Key k = ev->GetKey();

//		b2Vec2 vel = pBody->GetLinearVelocity();

		if (k == eKey::W)
		{
			SetState(Runing);
			fUpDownMove = -1;
		}

		if (k == eKey::A)
		{
			SetState(Runing);
			fMove = -1;
		}

		if (k == eKey::D)
		{
			SetState(Runing);
			fMove = 1;
		}

		if (k == eKey::S)
		{
			SetState(Runing);
			fUpDownMove = 1;
		}

		if(k == eKey::E && bIsTargetReachable)
		{
			SetState(Attacking);
			Attack();
		}
	}

	return true;
}

bool Player3Entity::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	if (this->bIsActive && this->bIsInputEnabled)
	{
		Key k = ev->GetKey();

		b2Vec2 vel = pBody->GetLinearVelocity();
		vel.x = 0;
		vel.y = 0;

		// Remove the directions
		if (k == eKey::W)
		{
			pBody->SetLinearVelocity(vel);
			fUpDownMove = 0;
		}

		if (k == eKey::A)
		{
			pBody->SetLinearVelocity(vel);
			fMove = 0;
		}

		if (k == eKey::D)
		{
			pBody->SetLinearVelocity(vel);
			fMove = 0;
		}

		if (k == eKey::S)
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

void Player3Entity::Update(f32 dt)
{
	PlayerEntity::Update(dt);

	// Search a nerby player
	if (pTarget == nullptr)
		pTarget = static_cast<PlayerEntity *>(gWorldManager->FindEntityByClassName("Player1"));

	if (pTarget != nullptr)
	{
		/*auto dir = pTarget->GetBodyPosition() - pBody->GetPosition();
		auto distance = dir.Normalize();

		if (distance <= 0.5f)
		{*/
			bIsTargetReachable = true;
		/*}
		else
		{
			bIsTargetReachable = false;
		}*/
	}
}
