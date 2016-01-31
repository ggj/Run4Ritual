#ifndef MUMMYENTITY_H
#define MUMMYENTITY_H

#include "enemyentity.h"
#include "playerentity.h"

class MummyEntity : public EnemyEntity
{
public:
	MummyEntity();

	void Update(f32 dt);
};

#endif // MUMMYENTITY_H
