#ifndef _AMULET_ENTITY_H
#define _AMULET_ENTITY_H

#include "itementity.h"

class AmuletEntity: public ItemEntity
{
	public:
		AmuletEntity();
		void OnCollision(const CollisionEvent &event);
};


#endif //_AMULET_ENTITY_H
