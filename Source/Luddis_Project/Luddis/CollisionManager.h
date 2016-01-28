
#ifndef INCLUDED_COLLISIONMANAGER
#define INCLUDED_COLLISIONMANAGER

#include "Collidable.h"
#include <vector>

class CollisionManager {
public:
	typedef std::vector<Collidable*> CollidableVector;
	CollisionManager();
	~CollisionManager();
	static CollisionManager& getInstance();
	void addCollidable(Collidable* collidable);
	CollidableVector mCollidables;
	void CollisionManager::detectCollisions();
	void CollisionManager::removeDeadCollidables();
};

#endif