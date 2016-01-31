#ifndef WOLFENTITY_H
#define WOLFENTITY_H

#include "enemyentity.h"
#include "playerentity.h"

class WolfEntity : public EnemyEntity
{
public:
	WolfEntity();

	void Update(f32 dt);
};

#endif // WOLFENTITY_H
