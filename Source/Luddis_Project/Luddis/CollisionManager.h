#ifndef INCLUDED_COLLISIONMANAGER
#define INCLUDED_COLLISIONMANAGER

#include <stack>
#include "CollidableEntity.h"
#include <vector>
#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
namespace sf{
	class RenderWindow;
}
#endif
class CollisionManager {
public:
	typedef std::vector<CollidableEntity*> CollidableVector;
	~CollisionManager();
	static CollisionManager& getInstance();
	void addCollidable(CollidableEntity* collidable);
	void detectCollisions();
	void removeDeadCollidables();
	void emptyVector();
	void narrowCollision(std::stack<std::pair<CollidableEntity*, CollidableEntity*>>& colliding);

#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	std::vector<sf::CircleShape> mDebugPoints;
	std::vector<sf::VertexArray> mDebugAxes;
	void drawHitboxes(sf::RenderWindow& aWindow) const;
#endif

	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(CollisionManager&) = delete;

private:
	CollidableVector mCollidables;
	CollisionManager();
};

#endif