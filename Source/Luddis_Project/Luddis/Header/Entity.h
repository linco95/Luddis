#ifndef INCLUDED_ENTITY
#define INCLUDED_ENTITY

#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp>

// Framåtdeklaration
namespace sf{
	class Clock;
	class Sprite;
}

class Entity : public sf::Transformable, public sf::Drawable {
public:
	Entity();
	virtual ~Entity();

	// Funktion för att uppdatera entiteten
	virtual void tick(const sf::Clock& gameTime) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual sf::Sprite& loadTexture() const = 0;
};

#endif