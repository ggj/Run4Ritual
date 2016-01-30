#ifndef _RED_SKULL_ENTITY_H
#define _RED_SKULL_ENTITY_H

#include "itementity.h"

class RedSkullEntity: public ItemEntity
{
	public:
		RedSkullEntity();
		void OnCollision(const CollisionEvent &event);
};

#endif // _RED_SKULL_ENTITY_H
