#ifndef INCLUDED_COLLIDABLE
#define INCLUDED_COLLIDABLE

#include <vector>
#include <SFML/Graphics/Sprite.hpp>

class Collidable {
public:

	enum Category{
		FRIEND,
		ENEMY,
		COLLECT,
		HAIR,
		BG
	};

	enum Type{
		REC,
		CIRCLE
	};
	Collidable();
	virtual ~Collidable();

	// Funktion för att uppdatera entiteten
	virtual Category getCollisionCategory() = 0;
	virtual Type getCollisionType() = 0;
	virtual void collide(Collidable *collidable) = 0;
	virtual bool isAlive() = 0;
	virtual sf::FloatRect getHitBox() = 0;
};

#endif