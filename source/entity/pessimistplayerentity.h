#ifndef _PESSIMIST_PLAYER_ENTITY_H
#define _PESSIMIST_PLAYER_ENTITY_H

#include "playerentity.h"

class PessimistPlayerEntity: public PlayerEntity
{
	public:
		PessimistPlayerEntity();

		// IEventInputKeyboardListener
		bool OnInputKeyboardPress(const EventInputKeyboard *ev) override;

		// IEventInputKeyboardListener
		bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;
};

#endif // _PESSIMIST_PLAYER_ENTITY_H
