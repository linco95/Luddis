#ifndef INCLUDED_ENTITY
#define INCLUDED_ENTITY

#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>


// Framåtdeklaration
namespace sf{
	class Time;
//	class Sprite;
}

class Entity : public sf::Transformable, public sf::Drawable {
public:	
	enum RenderLayer{
		BACKGROUND,
		OBSTACLES,
		ITEM,
		PLAYER,
		FOREGROUND,
		GUI
	};

	Entity();
	virtual ~Entity();

	// Funktion för att uppdatera entiteten
	virtual void tick(const sf::Time& deltaTime) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual bool isAlive() = 0;
	virtual RenderLayer getRenderLayer() const = 0;
};

#endif