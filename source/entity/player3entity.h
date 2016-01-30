#ifndef _PESSIMIST_PLAYER_ENTITY_H
#define _PESSIMIST_PLAYER_ENTITY_H

#include "playerentity.h"

class Player3Entity: public PlayerEntity
{
	public:
		Player3Entity();

		// IEventInputKeyboardListener
		bool OnInputKeyboardPress(const EventInputKeyboard *ev) override;

		// IEventInputKeyboardListener
		bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		void Update(f32 dt) override;
};

#endif // _PESSIMIST_PLAYER_ENTITY_H
