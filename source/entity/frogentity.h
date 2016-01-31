#ifndef FROGENTITY_H
#define FROGENTITY_H

#include "enemyentity.h"
#include "playerentity.h"

class FrogEntity : public EnemyEntity
{
public:
	FrogEntity();

	void Update(f32 dt);
};

#endif // FROGENTITY_H
