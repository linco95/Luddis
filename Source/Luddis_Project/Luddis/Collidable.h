#ifndef INCLUDED_COLLIDABLE
#define INCLUDED_COLLIDABLE

#include <vector>
#include <SFML/Graphics/Sprite.hpp>

class Collidable {
public:

	enum Category{
		FRIEND,
		ENEMY,
		BG
	};

	enum Type{
		REC,
		CIRCLE
	};
	Collidable();
	virtual ~Collidable();

	// Funktion för att uppdatera entiteten
	virtual Category getCategory() = 0;
	virtual Type getType() = 0;
	virtual void collide() = 0;
	virtual sf::FloatRect getHitBox() = 0;
};

#endif