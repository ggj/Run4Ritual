#ifndef _NECRONOMICON_ENTITY_H
#define _NECRONOMICON_ENTITY_H

#include "itementity.h"

class NecronomiconEntity: public ItemEntity
{
	public:
		NecronomiconEntity();
		void OnCollision(const CollisionEvent &event);
};

#endif // _NECRONOMICON_ENTITY_H
