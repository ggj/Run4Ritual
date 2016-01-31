#ifndef _PLAYER1_ENTITY_H
#define _PLAYER1_ENTITY_H

#include "playerentity.h"
#include "../manager/guimanager.h"

class Player1Entity: public PlayerEntity
{
	public:
		Player1Entity();

		// IEventInputKeyboardListener
		bool OnInputKeyboardPress(const EventInputKeyboard *ev) override;

		// IEventInputKeyboardListener
		bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		void Update(f32 dt) override;
		void Attack();

		inline void SetLife(const u32 life)
		{
			sPlayer.iLife = life;
			gGui->SetP1Life(life);
		}

		bool OnDamage(const b2Vec2 vec2Push, u32 amount);
};

#endif // _PLAYER1_ENTITY_H
