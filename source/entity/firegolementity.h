#ifndef FIREGOLEMENTITY_H
#define FIREGOLEMENTITY_H

#include "enemyentity.h"
#include "playerentity.h"

class FireGolemEntity : public EnemyEntity
{
public:
	FireGolemEntity();

	void Update(f32 dt);
};

#endif // FIREGOLEMENTITY_H
