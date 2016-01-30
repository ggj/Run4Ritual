#include "pathfindermanager.h"
#include "map/pathfind/AStarPathfinder.h"

PathfinderManager::PathfinderManager()
	: pPathfinder(nullptr)
{
}

PathfinderManager::~PathfinderManager()
{
	sdDelete(pPathfinder);
}

void PathfinderManager::Init(MapLayerTiled *mapBackground)
{
	// TODO - BUG here, change the parameter 3
	pPathfinder = sdNew(AStarPathfinder(false, false, 40, 3, mapBackground));
}


void PathfinderManager::Update(f32 dt)
{
	UNUSED(dt);
}

Path &PathfinderManager::Findpath(const vec3 &start, const vec3 &end, Path &path)
{
	return pPathfinder->FindPath(start, end, path);
}

