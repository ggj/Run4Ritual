#ifndef ZOMBIEENTITY_H
#define ZOMBIEENTITY_H

#include "enemyentity.h"
#include "playerentity.h"

class ZombieEntity : public EnemyEntity
{
public:
	ZombieEntity();

	void Update(f32 dt);
};

#endif // ZOMBIEENTITY_H
