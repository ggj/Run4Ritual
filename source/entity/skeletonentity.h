#ifndef SKELETONENTITY_H
#define SKELETONENTITY_H

#include "enemyentity.h"
#include "playerentity.h"

class SkeletonEntity : public EnemyEntity
{
public:
	SkeletonEntity();

	void Update(f32 dt);
};

#endif // SKELETONENTITY_H
