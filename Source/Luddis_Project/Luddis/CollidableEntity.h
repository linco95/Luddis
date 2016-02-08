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
		BG_DAMAGE
	};

	enum Type{
		REC,
		CIRCLE
	};
	CollidableEntity();
	virtual ~CollidableEntity();

	// Funktion för att uppdatera entiteten
	virtual Category getCollisionCategory() = 0;
	virtual Type getCollisionType() = 0;
	virtual void collide(CollidableEntity *collidable) = 0;
	virtual sf::FloatRect getHitBox() = 0;
	virtual sf::Shape* getNarrowHitbox() const = 0;
};

#endif