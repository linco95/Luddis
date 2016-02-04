
#ifndef INCLUDED_COLLISIONMANAGER
#define INCLUDED_COLLISIONMANAGER

#include "Collidable.h"
#include <vector>
#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
namespace sf{
	class RenderWindow;
}
#endif
class CollisionManager {
public:
	typedef std::vector<Collidable*> CollidableVector;
	~CollisionManager();
	static CollisionManager& getInstance();
	void addCollidable(Collidable* collidable);
	void detectCollisions();
	void removeDeadCollidables();

#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	void drawHitboxes(sf::RenderWindow& aWindow) const;
#endif

	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(CollisionManager&) = delete;
private:
	CollidableVector mCollidables;
	CollisionManager();
};

#endif