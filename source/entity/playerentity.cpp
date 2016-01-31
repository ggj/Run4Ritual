#include <Box2D/Common/b2Math.h>
#include "playerentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"
#include "../manager/guimanager.h"
#include "../gameflow.h"

ENTITY_CREATOR("Player", PlayerEntity)

PlayerEntity::PlayerEntity()
	: SpriteEntity("Player", "Player")
	, pBody(nullptr)
	, pImpactFX(nullptr)
	, vPlayerVectorDirection()
	, eItem(ItemTypes::None)
	, iPreviousState(Waiting)
	, iCurrentState(Waiting)
	, fVelocity(0.0f)
	, fMove(0.0f)
	, fUpDownMove(0.0f)
	, fInvicibleTime(0.0f)
	, bIsActive(false)
	, bIsInputEnabled(true)
	, pTarget(nullptr)
	, bIsTargetReachable(false)
{
}

PlayerEntity::PlayerEntity(const char *className, const char *spriteName, bool bIsActive)
	: SpriteEntity(className, spriteName)
	, pBody(nullptr)
	, pImpactFX(nullptr)
	, vPlayerVectorDirection()
	, eItem(ItemTypes::None)
	, iPreviousState(Waiting)
	, iCurrentState(Waiting)
	, fVelocity(0.0f)
	, fMove(0.0f)
	, fUpDownMove(0.0f)
	, fInvicibleTime(0.0f)
	, bIsActive(bIsActive)
	, bIsInputEnabled(true)
{
}

PlayerEntity::~PlayerEntity()
{
	gScene->Remove(pImpactFX);
	sdDelete(pImpactFX);

	pInput->RemoveKeyboardListener(this);
	gPhysics->DestroyBody(pBody);
	pBody = nullptr;
}

void PlayerEntity::Load(MetadataObject &metadata, SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
	pSprite->SetZ(-10);

	pImpactFX = sdNew(Sprite(*static_cast<Sprite *>(sprites->GetChildByName("ImpactFX"))));

	pImpactFX->SetPosition(0, 0);
	pImpactFX->SetVisible(false);
	gScene->Add(pImpactFX);

	b2Vec2 customSize(32, 32);

	pBody = gPhysics->CreateBody(pSprite, &customSize);
	pBody->SetFixedRotation(true);
	pBody->GetFixtureList()->SetUserData(this);

	pInput->AddKeyboardListener(this);
	//fVelocity = 2.0f;
	vPlayerVectorDirection = VECTOR_ZERO;
}

vec3 PlayerEntity::GetPosition()
{
	return pSprite->GetPosition();
}

b2Vec2 PlayerEntity::GetBodyPosition() const
{
	return pBody->GetPosition();
}

Sprite *PlayerEntity::GetSprite() const
{
	return pSprite;
}

void PlayerEntity::Teleport(const b2Vec2 &position)
{
	pBody->SetTransform(position, pBody->GetAngle());

	fMove= 0;
	fUpDownMove= 0;
	this->SetState(Waiting);

	gSoundManager->Play(SND_TELEPORT);
}

void PlayerEntity::Update(f32 dt)
{
	pImpactFX->SetPosition(pSprite->GetPosition() + vec3(0, 0, 0));

	b2Vec2 vel = pBody->GetLinearVelocity();

	if (fInvicibleTime > 0)
	{
		pSprite->SetVisible(!pSprite->IsVisible());

		fInvicibleTime -= dt;
		if (fInvicibleTime <= 0)
		{
			pSprite->SetVisible(true);
			fInvicibleTime = 0;
			if (this->bIsActive)
			{
				this->bIsInputEnabled = true;
				this->StopPlayerMovement();
				SetState(Waiting);
				pImpactFX->SetVisible(false);
			}
		}
	}

	if (fMove != 0)
	{
		// Change sprite direction based on movement
		this->GetSprite()->SetScaleX(-fMove);

		vel.x = fVelocity * fMove;
		pBody->SetLinearVelocity(vel);
	}

	if (fUpDownMove != 0)
	{
		vel.y = fVelocity * fUpDownMove;
		pBody->SetLinearVelocity(vel);
	}

	if (iCurrentState == iPreviousState)
		return;

	if (iCurrentState == Runing)
	{
		pSprite->SetAnimation("Run");
	}
	else
	{
		pSprite->SetAnimation("Idle");
	}

	iPreviousState = iCurrentState;
}

void PlayerEntity::SetItem(ItemTypes::Enum item)
{
	eItem = item;
	pImpactFX->SetVisible(eItem == ItemTypes::Text);
}

ItemTypes::Enum PlayerEntity::GetItem() const
{
	return eItem;
}

void PlayerEntity::SetIsActive(bool isActive)
{
	bIsActive = isActive;
}

void PlayerEntity::SetIsInputEnabled(bool isInputEnabled)
{
	bIsInputEnabled = isInputEnabled;
}

bool PlayerEntity::GetIsInputEnabled() const
{
	return bIsInputEnabled;
}

void PlayerEntity::StopPlayerMovement()
{
	b2Vec2 vel = pBody->GetLinearVelocity();
	vel.x = 0;
	vel.y = 0;

	pBody->SetLinearVelocity(vel);
	fUpDownMove = 0;
	fMove = 0;
}

bool PlayerEntity::GetIsActive()
{
	return bIsActive;
}

f32 PlayerEntity::GetInvencibleTime()
{
	return fInvicibleTime;
}

void PlayerEntity::SetState(int newState)
{
	iPreviousState = iCurrentState;
	iCurrentState = newState;
}

String PlayerEntity::GetDisplayName() const
{
	return sPlayer.displayName;
}

u32 PlayerEntity::GetLevel() const
{
	return sPlayer.iLevel;
}
void PlayerEntity::SetLevel(const u32 level)
{
	sPlayer.iLevel = level;
}

u32 PlayerEntity::GetXP() const
{
	return sPlayer.iXP;
}
void PlayerEntity::SetXP(const u32 xp)
{
	sPlayer.iXP = xp;
}

u32 PlayerEntity::GetAttackPower() const
{
	return sPlayer.iAttackPower;
}
void PlayerEntity::SetAttackPower(const u32 attackPower)
{
	sPlayer.iAttackPower = attackPower;
}

u32 PlayerEntity::GetGold() const
{
	return sPlayer.iGold;
}
void PlayerEntity::SetGold(const u32 gold)
{
	sPlayer.iGold = gold;
	gGui->SetGold(gold);
}

u32 PlayerEntity::GetLife() const
{
	return sPlayer.iLife;
}

void PlayerEntity::RemoveLife()
{
	sPlayer.iLife--;
}

u32 PlayerEntity::GetLifeTotal() const
{
	return sPlayer.iLifeTotal;
}
void PlayerEntity::SetLifeTotal(const u32 lifeTotal)
{
	sPlayer.iLifeTotal = lifeTotal;
}
void PlayerEntity::RemoveLifeTotal()
{
	sPlayer.iLifeTotal--;
}

u32 PlayerEntity::GetStamina() const
{
	return sPlayer.iStamina;
}
void PlayerEntity::SetStamina(const u32 stamina)
{
	sPlayer.iStamina = stamina;
	gGui->SetStamina(stamina, this->sPlayer.iStaminaTotal);
}
void PlayerEntity::RemoveStamina()
{
	sPlayer.iStamina--;
}

u32 PlayerEntity::GetStaminaTotal() const
{
	return sPlayer.iStaminaTotal;
}
void PlayerEntity::SetStaminaTotal(const u32 staminaTotal)
{
	sPlayer.iStaminaTotal = staminaTotal;
}
void PlayerEntity::RemoveStaminaTotal()
{
	sPlayer.iStaminaTotal--;
}

u32 PlayerEntity::GetDefensePower() const
{
	return sPlayer.iDefensePower;
}

void PlayerEntity::SetDefensePower(u32 defensePower)
{
	sPlayer.iDefensePower = defensePower;
}

bool PlayerEntity::IsAmulet() const
{
	return sPlayer.bAmulet;
}

void PlayerEntity::SetAmulet(bool amulet)
{
	sPlayer.bAmulet = amulet;
}

bool PlayerEntity::IsNecronomicon() const
{
	return sPlayer.bNecronomicon;
}

void PlayerEntity::SetNecronomicon(bool necronomicon)
{
	sPlayer.bNecronomicon = necronomicon;
}

bool PlayerEntity::IsRedSkull() const
{
	return sPlayer.bRedSkull;
}

void PlayerEntity::SetRedSkull(bool redSkull)
{
	sPlayer.bRedSkull = redSkull;
}

bool PlayerEntity::IsCrystalBall() const
{
	return sPlayer.bCrystalBall;
}

void PlayerEntity::SetCrystalBall(bool crystalBall)
{
	sPlayer.bCrystalBall = crystalBall;
}


void PlayerEntity::OnCollect(ItemTypes::Enum item, u32 amount)
{
	// Play collect sound
	gSoundManager->Play(SND_POWERUP);

	if(item == ItemTypes::CrystalBall)
	{
		this->SetCrystalBall(true);
	}

	if(item == ItemTypes::RedSkull)
	{
		this->SetRedSkull(true);
	}

	if(item == ItemTypes::Necronomicon)
	{
		this->SetNecronomicon(true);
	}

	if(item == ItemTypes::Amulet)
	{
		this->SetAmulet(true);
	}

}

u32 PlayerEntity::GiveKey()
{
	auto key = sPlayer.iKey;
	sPlayer.iKey = 0;
	return key;
}

void PlayerEntity::ReceiveKey(u32 key)
{
	sPlayer.iKey = key;
}

void PlayerEntity::OnCollision(const CollisionEvent &event)
{
	if (event.GetType() == CollisionEventType::OnEnter)
	{
		Entity *other = event.GetOtherEntity();
		if (other != nullptr && other->GetClassName() == "Trigger")
		{
			//gGameScene->UseKey(this->GiveKey());

			// Set to game over
			if (sPlayer.bCrystalBall && sPlayer.bAmulet && sPlayer.bNecronomicon && sPlayer.bRedSkull)
				gGameData->SetIsGameOver(true);
		}
	}
}

bool PlayerEntity::OnDamage(const b2Vec2 vec2Push, u32 amount)
{
	UNUSED(vec2Push)
	UNUSED(amount)
	return false;
}
