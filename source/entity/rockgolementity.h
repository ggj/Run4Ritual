#ifndef ROCKGOLEMENTITY_H
#define ROCKGOLEMENTITY_H

#include "enemyentity.h"
#include "playerentity.h"

class RockGolemEntity : public EnemyEntity
{
public:
	RockGolemEntity();

	void Update(f32 dt);
};

#endif // ROCKGOLEMENTITY_H
