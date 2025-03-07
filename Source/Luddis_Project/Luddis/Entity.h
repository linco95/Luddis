#ifndef INCLUDED_ENTITY
#define INCLUDED_ENTITY

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "Renderer.h"

// Forwarddeclaration
namespace sf{
	class Time;
}

class Entity : public sf::Transformable, public sf::Drawable {
public:	
	//enum RenderLayer{
	//	BACKGROUND,
	//	OBSTACLES,
	//	ITEM,
	//	PLAYER,
	//	FOREGROUND,
	//	MOUSE
	//};

	Entity();
	virtual ~Entity();

	// Update the entity
	virtual void tick(const sf::Time& deltaTime) = 0;
	// Draw the entity
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	// Check if an entity is alive
	virtual bool isAlive() const = 0;
	// Determines in what order entities are drawn
	virtual Renderer::RenderLayer getRenderLayer() const = 0;
	// Check if an entity is active
	virtual bool isActive() const = 0;
	// Set an entities "active" state
	virtual void setActive(const bool& active) = 0;
	// Stun the entity
	virtual void stun(const sf::Time& deltaTime) = 0;
};

#endif