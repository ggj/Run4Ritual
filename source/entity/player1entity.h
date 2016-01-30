#ifndef _PLAYER1_ENTITY_H
#define _PLAYER1_ENTITY_H

#include "playerentity.h"

class Player1Entity: public PlayerEntity
{
	public:
		Player1Entity();

		// IEventInputKeyboardListener
		bool OnInputKeyboardPress(const EventInputKeyboard *ev) override;

		// IEventInputKeyboardListener
		bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;
};

#endif // _PLAYER1_ENTITY_H
