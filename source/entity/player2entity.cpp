#include "player2entity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"
#include "../gameflow.h"

ENTITY_CREATOR("Player2", Player2Entity)

Player2Entity::Player2Entity()
	: PlayerEntity("Player2", "Player2", true)
{
	fVelocity = 2.5f;

	sPlayer.iKey = 1.5f;
	sPlayer.displayName = "Player2";
	sPlayer.iLevel = 1;
	sPlayer.iXP = 80;
	sPlayer.iAttackPower = 15;
	sPlayer.iDefensePower = 5;
	sPlayer.iGold = 10;
	sPlayer.iLife = 3;
	sPlayer.iLifeTotal = 22;
	sPlayer.iStamina = 5;
	sPlayer.iStaminaTotal = 5;
	bIsActive = true;
	pTarget = nullptr;
}

bool Player2Entity::OnInputKeyboardPress(const EventInputKeyboard *ev)
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

bool Player2Entity::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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

void Player2Entity::Update(f32 dt)
{
	PlayerEntity::Update(dt);

	// Search a nerby player
	if (pTarget == nullptr)
		pTarget = static_cast<PlayerEntity *>(gWorldManager->FindEntityByClassName("Player1"));

	if (pTarget != nullptr)
	{
		auto dir = pTarget->GetBodyPosition() - pBody->GetPosition();
		auto distance = dir.Normalize();

		if (distance <= 0.5f)
		{
			bIsTargetReachable = true;
		}
		else
		{
			bIsTargetReachable = false;
		}
	}
}

void Player2Entity::Attack()
{
	Log("%s: Attack", pTarget->GetClassName().c_str());
	pTarget->OnDamage(b2Vec2(0, 0), 1);
}

bool Player2Entity::OnDamage(const b2Vec2 vec2Push, u32 amount)
{
	// Play damage sound
	gSoundManager->Play(SND_DAMAGE);

	pImpactFX->SetVisible(true);
	pSprite->SetAnimation("SingleSlash");

	// Create the ghost effect
	if (fInvicibleTime > 0)
		return false;

	fInvicibleTime = 1.0f;
	//pText->SetVisible(true);

	// Receive the damage
	u32 life = this->GetLife() - amount;

	if ((int)life > 0)
		this->SetLife(life);
	else
		gGameData->SetIsGameOver(true);

	return true;
}
