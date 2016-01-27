
#ifndef INCLUDED_COLLISIONMANAGER
#define INCLUDED_COLLISIONMANAGER

#include "Collidable.h"
#include <vector>

class CollisionManager {
public:
	CollisionManager();
	~CollisionManager();
	void addCollidable(Collidable* collidable);
	Collidable::CollidableVector mCollidables;
};

#endif