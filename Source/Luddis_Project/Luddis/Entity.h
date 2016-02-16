#ifndef INCLUDED_ENTITY
#define INCLUDED_ENTITY

#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>


// Forwarddeclaration
namespace sf{
	class Time;
}

class Entity : public sf::Transformable, public sf::Drawable {
public:	
	enum RenderLayer{
		BACKGROUND,
		OBSTACLES,
		ITEM,
		PLAYER,
		FOREGROUND
	};

	Entity();
	virtual ~Entity();

	// Udate the entity
	virtual void tick(const sf::Time& deltaTime) = 0;
	// Draw the entity
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	// Check if an entity is alive
	virtual bool isAlive() const = 0;
	// Determines in what order entities are drawn
	virtual RenderLayer getRenderLayer() const = 0;
	// Check if an entity is active
	virtual bool isActive() const = 0;
	// Set an entities "active" state
	virtual void setActive(const bool& active) = 0;
};

#endif