#ifndef SNAKEENTITY_H
#define SNAKEENTITY_H

#include "enemyentity.h"
#include "playerentity.h"

class SnakeEntity : public EnemyEntity
{
public:
	SnakeEntity();

	void Update(f32 dt);
};

#endif // SNAKEENTITY_H
