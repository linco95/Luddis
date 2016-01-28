#ifndef INCLUDED_LEVEL
#define INCLUDED_LEVEL

#include <SFML\Graphics.hpp>
#include "Entity.h"
class Level : public Entity {

public:

	Level();
	~Level();


	void initializeLevel(sf::RenderWindow& aWindow, sf::Transformable* aTarget);

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	Entity::RenderLayer getRenderLayer() const override;

private:
	sf::Transformable* mTarget;
	sf::View mView;
	sf::Sprite mBackground;
};

#endif