#ifndef _CRYSTAL_BALL_ENTITY_H
#define _CRYSTAL_BALL_ENTITY_H

#include "itementity.h"

class CrystalBallEntity: public ItemEntity
{
	public:
		CrystalBallEntity();
		void OnCollision(const CollisionEvent &event);
};

#endif // _CRYSTAL_BALL_ENTITY_H
