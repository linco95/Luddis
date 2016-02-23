#ifndef INCLUDED_COLLIDABLE_ENTITY
#define INCLUDED_COLLIDABLE_ENTITY

#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include "Entity.h"
#include <SFML/Graphics/Shape.hpp>

class CollidableEntity : public Entity {
public:

	enum Category{
		FRIEND,
		ENEMY,
		COLLECT,
		HAIR,
		ENEMY_PROJECTILE,
		ENEMY_STUN,
		BG_SOLID,
		BG_DAMAGE,
		EVENTZONE
	};

	enum Type{
		REC,
		CIRCLE
	};
	CollidableEntity();
	virtual ~CollidableEntity();

	// Check what kind of object this is
	virtual Category getCollisionCategory() = 0;
	// Check the shape of the object
	virtual Type getCollisionType() = 0;
	// Collide the object with "collidable"
	virtual void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) = 0;
	virtual sf::FloatRect getHitBox() = 0;
	virtual sf::Shape* getNarrowHitbox() const = 0;
};

#endif