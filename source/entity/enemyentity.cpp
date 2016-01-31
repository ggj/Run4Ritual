#include "enemyentity.h"
#include "entityfactory.h"
#include "../scene/gamescene.h"
#include "../util/sounds.h"
#include "../manager/guimanager.h"
#include <cmath>

#define PIX2M		0.01f
#define M2PIX		(1.0f / PIX2M)

ENTITY_CREATOR("Enemy", EnemyEntity)

EnemyEntity::EnemyEntity()
	: SpriteEntity("Enemy", "Enemy")
	, pTarget(nullptr)
	, pBody(nullptr)
	, fInvicibleTime(0.0f)
	, bPlayerLock(false)
	, bIsDead(false)
	, bIsPlayerFound(false)
	, cPath()
{
	fVelocity = 0.55f;
	sEnemy.displayName = "Enemy";
}

EnemyEntity::~EnemyEntity()
{
	gPhysics->DestroyBody(pBody);
	pBody = nullptr;
}

void EnemyEntity::Load(MetadataObject &metadata, SceneNode *sprites)
{
	SpriteEntity::Load(metadata, sprites);
	pSprite->SetZ(-10);
	clSensor.Load(metadata, this);

	if (!metadata.GetProperty("DisplayName").empty())
		sEnemy.displayName = metadata.GetProperty("DisplayName");
	else
		sEnemy.displayName = "Enemy";

	if (!metadata.GetProperty("EnemyId").empty())
		sEnemy.iEnemyId = std::stoi(metadata.GetProperty("EnemyId"));
	else
		sEnemy.iEnemyId = 0;

	if (!metadata.GetProperty("Level").empty())
		sEnemy.iLevel = std::stoi(metadata.GetProperty("Level"));
	else
		sEnemy.iLevel = 1;

	if (!metadata.GetProperty("AttackPower").empty())
		sEnemy.iAttackPower = std::stoi(metadata.GetProperty("AttackPower"));
	else
		sEnemy.iAttackPower = 3;

	if (!metadata.GetProperty("DefensePower").empty())
		sEnemy.iDefensePower = std::stoi(metadata.GetProperty("DefensePower"));
	else
		sEnemy.iDefensePower = 2;

	if (!metadata.GetProperty("Life").empty())
		sEnemy.iLife = std::stoi(metadata.GetProperty("Life"));
	else
		sEnemy.iLife = 5;

	if (!metadata.GetProperty("LifeTotal").empty())
		sEnemy.iLifeTotal = std::stoi(metadata.GetProperty("LifeTotal"));
	else
		sEnemy.iLifeTotal = 5;

	b2Vec2 customSize(16, 16);

	pBody = gPhysics->CreateBody(pSprite, &customSize);
	pBody->SetFixedRotation(true);
	pBody->GetFixtureList()->SetUserData(this);
}

void EnemyEntity::Update(f32 dt)
{
	if (!pBody)
	//	return;

	//b2Vec2 vel = pBody->GetLinearVelocity();
	//pBody->SetLinearVelocity(vel);

	if (fInvicibleTime > 0)
	{
		pSprite->SetVisible(!pSprite->IsVisible());

		fInvicibleTime -= dt;
		if (fInvicibleTime <= 0)
		{
			pSprite->SetVisible(true);
			fInvicibleTime = 0;
		}
	}

	// Search a nerby player
	if (pTarget == nullptr)
		pTarget = static_cast<Player3Entity *>(gWorldManager->FindEntityByClassName("Player3"));

	if (pTarget != nullptr)
	{
		auto dir = pTarget->GetBodyPosition() - pBody->GetPosition();
		auto distance = dir.Normalize();


		if (bPlayerLock)
		{
			if (distance > 2.0f)
			{
				Log("Perdeu o player");
				this->StopToFollow();
			}
			else
			{
				this->FindPathToPlayer();
			}
		}
		else
		{
			if (distance <= 2.0f)
			{
				Log("Achou o player");
				bPlayerLock = true;
			}
		}

	}
}


void EnemyEntity::StopToFollow()
{
	bPlayerLock = false;

	// Clear the stack
	while ( ! cPath.GetPositionSteps().empty() )
	{
		cPath.GetPositionSteps().pop();
	}

	// Clear the stack
	while ( ! cPath.GetDirectionSteps().empty() )
	{
		cPath.GetDirectionSteps().pop();
	}

	// Stop the enemy
	b2Vec2 vel = pBody->GetLinearVelocity();
	vel.x = 0;
	vel.y = 0;

	// Make the enemy follow the player
	pBody->SetLinearVelocity(vel);
}

void EnemyEntity::FindPathToPlayer()
{
	// Find path to the player
	gPathfinderManager->Findpath(pSprite->GetPosition(), pTarget->GetSprite()->GetPosition(), cPath);
	bIsPlayerFound = false;

	if (cPath.GetDirectionSteps().empty())
	{
		pTarget = static_cast<Player3Entity *>(gWorldManager->FindEntityByClassName("Player3"));

		Log("Nao esta achando as direcoes para o player");
		return;
	}

	// Find the steps to follow
	auto dir = cPath.GetDirectionSteps().top();

	b2Vec2 vel = pBody->GetLinearVelocity();
	vel.x = fVelocity * dir.x;
	vel.y = fVelocity * dir.y;

	// Make the enemy follow the player
	pBody->SetLinearVelocity(vel);
	//Log("Path: %f, %f", vel.x, vel.y);
	Log("GetDirectionSteps: %d ", cPath.GetDirectionSteps().size());
	Log("GetPositionSteps: %d ", cPath.GetPositionSteps().size());

	while (!cPath.GetDirectionSteps().empty())
		cPath.GetDirectionSteps().pop();
}

void EnemyEntity::OnCollision(const CollisionEvent &event)
{
	if (event.GetType() == CollisionEventType::OnEnter && !bIsDead)
	{
		Log("ENEMY colidiu");

		Entity *other = event.GetOtherEntity();
		if ((other != nullptr && other->GetClassName() == "Player1") ||
			(other != nullptr && other->GetClassName() == "Player2") ||
			(other != nullptr && other->GetClassName() == "Player3") ||
			(other != nullptr && other->GetClassName() == "Player4"))
		{
			//Player1Entity *player = static_cast<Player1Entity *>(other);

			//s32 damageToPlayer = (player->GetDefensePower() - sEnemy.iAttackPower) + (rand() % 3 + 1);
			//if (damageToPlayer < 0)
			//	damageToPlayer = 0;

			//Do damage to the player
			//player->OnDamage(vecToPush, u32(damageToPlayer));

			//s32 damageEnemyBase = player->GetAttackPower() - sEnemy.iDefensePower + (rand() % 3 + 1);
			//if (damageEnemyBase < 0)
			//	damageEnemyBase = 0;

			//Receive damage
			//this->OnDamage(u32(damageEnemyBase));
		}
	}
}

bool EnemyEntity::OnDamage(u32 amount)
{
	// Play damage sound
	gSoundManager->Play(SND_DAMAGE);

	if (fInvicibleTime > 0)
		return false;

	// Receive the damage
	this->SetLife(this->GetLife() - amount);

	if((int)this->GetLife() <= 0)
	{
		// Disable item
		this->pSprite->SetVisible(false);

		// Add body to a list to remove
		gPhysics->AddBodyToRemove(pBody);
		pBody = nullptr;
		bIsDead = true;
		gGui->SelectEnemy();
	}
	else
		fInvicibleTime = 3;

	return true;
}

String EnemyEntity::GetDisplayName() const
{
	return sEnemy.displayName;
}

void EnemyEntity::SetDisplayName(const String &displayName)
{
	//gGui->SetEnemyName(displayName);
}

u32 EnemyEntity::GetLevel() const
{
	return sEnemy.iLevel;
}

void EnemyEntity::SetLevel(u32 level)
{
	sEnemy.iLevel = level;
	gGui->SetEnemyLevel(level);
}

u32 EnemyEntity::GetLife() const
{
	return sEnemy.iLife;
}

void EnemyEntity::SetLife(u32 life)
{
	sEnemy.iLife = life;
	gGui->SetEnemyLife(life, this->sEnemy.iLifeTotal);
}
