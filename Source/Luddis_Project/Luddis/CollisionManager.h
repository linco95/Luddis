
#ifndef INCLUDED_COLLISIONMANAGER
#define INCLUDED_COLLISIONMANAGER

#include "Collidable.h"
#include <vector>

class CollisionManager {
public:
	CollisionManager();
	~CollisionManager();
	static CollisionManager& getInstance();
	void addCollidable(Collidable* collidable);
	Collidable::CollidableVector mCollidables;
	void CollisionManager::detectCollisions();
};

#endif