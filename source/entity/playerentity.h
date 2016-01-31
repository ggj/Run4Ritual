#ifndef _PLAYER_ENTITY_H
#define _PLAYER_ENTITY_H

#include <Box2D/Box2D.h>
#include "../defines.h"
#include "spriteentity.h"

namespace ItemTypes
{
	enum Enum
	{
		None,
		Text,
		CrystalBall,
		RedSkull,
		Necronomicon,
		Amulet
	};
}

class PlayerEntity: public SpriteEntity,
					public IEventInputKeyboardListener
{
	public:
		PlayerEntity();
		PlayerEntity(const char *className, const char *spriteName, bool bIsActive);

		virtual ~PlayerEntity();

		void Load(MetadataObject &metadata, SceneNode *sprites);
		void Update(f32 dt);

		void OnCollision(const CollisionEvent &event);

		vec3 GetPosition();
		Sprite *GetSprite() const;
		b2Vec2 GetBodyPosition() const;

		void SetItem(ItemTypes::Enum item);
		ItemTypes::Enum GetItem() const;

		void Teleport(const b2Vec2 &position);
		virtual bool OnDamage(const b2Vec2 vec2Push, u32 amount);
		void OnCollect(ItemTypes::Enum item, u32 amount);

		void StopPlayerMovement();

		void SetIsActive(bool isActive);
		bool GetIsActive();

		void SetIsInputEnabled(bool isKeyboardEnabled);
		bool GetIsInputEnabled() const;

		String GetDisplayName() const;

		u32 GetLevel() const;
		void SetLevel(u32);

		u32 GetXP() const;
		void SetXP(u32);

		u32 GetAttackPower() const;
		void SetAttackPower(u32);

		u32 GetGold() const;
		void SetGold(u32);

		u32 GetLife() const;
		void RemoveLife();

		u32 GetLifeTotal() const;
		void SetLifeTotal(u32);
		void RemoveLifeTotal();

		u32 GetStamina() const;
		void SetStamina(u32);
		void RemoveStamina();

		u32 GetStaminaTotal() const;
		void SetStaminaTotal(u32);
		void RemoveStaminaTotal();

		//--

		bool IsAmulet() const;
		void SetAmulet(bool);

		bool IsNecronomicon() const;
		void SetNecronomicon(bool);

		bool IsRedSkull() const;
		void SetRedSkull(bool);

		bool IsCrystalBall() const;
		void SetCrystalBall(bool);

		//--
		u32 GiveKey();
		void ReceiveKey(u32 key);

		u32 GetDefensePower() const;
		void SetDefensePower(u32 defensePower);

		struct PlayerData
		{
			String displayName;
			u32 iLevel;
			u32 iXP;
			u32 iAttackPower;
			u32 iDefensePower;
			u32 iGold;
			u32 iLife;
			u32 iLifeTotal;
			u32 iStamina;
			u32 iStaminaTotal;
			u32 iKey;
			bool bAmulet;
			bool bNecronomicon;
			bool bRedSkull;
			bool bCrystalBall;
			bool bGameOver;
		} sPlayer;

	protected:
		PlayerEntity *pTarget;
		b2Body *pBody;
		Sprite *pImpactFX;
		vec3 vPlayerVectorDirection;

		ItemTypes::Enum eItem;

		enum eAnimationStates {Waiting = 0, Runing = 1, Attacking = 2};
		s32 iPreviousState;
		s32 iCurrentState;

		f32 fVelocity;
		f32 fMove;
		f32 fUpDownMove;
		f32 fInvicibleTime;
		bool bIsTargetReachable;

		bool bIsActive;
		bool bIsInputEnabled;
		void SetState(int newState);
};

#endif
