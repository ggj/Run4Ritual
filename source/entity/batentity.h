#ifndef BATENTITY_H
#define BATENTITY_H

#include "enemyentity.h"
#include "playerentity.h"

class BatEntity : public EnemyEntity
{
public:
	BatEntity();

	void Update(f32 dt);
};

#endif // BATENTITY_H
