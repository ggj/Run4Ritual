#ifndef BEETLEENTITY_H
#define BEETLEENTITY_H

#include "enemyentity.h"
#include "playerentity.h"

class BeetleEntity : public EnemyEntity
{
public:
	BeetleEntity();

	void Update(f32 dt);
};

#endif // BEETLEENTITY_H
