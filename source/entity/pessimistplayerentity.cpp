#include "pessimistplayerentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"

ENTITY_CREATOR("PessimistPlayer", PessimistPlayerEntity)

PessimistPlayerEntity::PessimistPlayerEntity()
	: PlayerEntity("PessimistPlayer", "PessimistPlayer", false)
{
	fVelocity = 0.7f;

	sPlayer.iKey = 3;
	sPlayer.displayName = "Pessimist";
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

bool PessimistPlayerEntity::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	if (this->bIsActive && this->bIsInputEnabled)
	{
		Key k = ev->GetKey();

//		b2Vec2 vel = pBody->GetLinearVelocity();

		if (k == eKey::W && iCurrentState != Jump)
		{
			SetState(Run);
			fUpDownMove = -1;
		}

		if (k == eKey::A)
		{
			SetState(Run);
			fMove = -1;
		}

		if (k == eKey::D)
		{
			SetState(Run);
			fMove = 1;
		}

		if (k == eKey::S)
		{
			SetState(Run);
			fUpDownMove = 1;
		}
	}

	return true;
}

bool PessimistPlayerEntity::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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

		if (k == eKey::Space)
		{
			gGameScene->ChangePlayer(this->GetClassName());
			return false;
		}

		if (fUpDownMove == 0 && fMove == 0)
		{
			SetState(Idle);
		}
	}

	return true;
}
