#ifndef _PLAYER2_ENTITY_H
#define _PLAYER2_ENTITY_H

#include "playerentity.h"
#include "../manager/guimanager.h"

class Player2Entity: public PlayerEntity
{
	public:
		Player2Entity();

		// IEventInputKeyboardListener
		bool OnInputKeyboardPress(const EventInputKeyboard *ev) override;

		// IEventInputKeyboardListener
		bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		void Update(f32 dt) override;
		void Attack();

		inline void SetLife(const u32 life)
		{
			sPlayer.iLife = life;
			gGui->SetP2Life(life);
		}

		bool OnDamage(const b2Vec2 vec2Push, u32 amount);
};

#endif // _PLAYER2_ENTITY_H
