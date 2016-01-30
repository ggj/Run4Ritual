#ifndef _DEATHENTITY_H
#define _DEATHENTITY_H

#include "spriteentity.h"
#include "../util/collisionsensor.h"

class Player1Entity;

class DeathEntity: public SpriteEntity
{
	public:
		DeathEntity();
		virtual ~DeathEntity();

		virtual void Load(MetadataObject &metadata, SceneNode *sprites);
		virtual void Update(f32 dt);

		vec3 GetPosition();
		Sprite *GetSprite() const;

		virtual void OnCollision(const CollisionEvent &event);
		virtual void Activate();

	private:
		CollisionSensor clSensor;
		f32 fSleepConfig;
		f32 fSleep;
		f32 fSpeedFactor;

};

#endif
