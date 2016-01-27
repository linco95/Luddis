#ifndef INCLUDED_LEVEL
#define INCLUDED_LEVEL

#include <SFML\Graphics.hpp>

class Level : public sf::Drawable {

public:

	Level();
	~Level();


	void initializeLevel(sf::RenderWindow& aWindow, sf::Transformable* aTarget);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Transformable* mTarget;
	sf::View mView;
	sf::Sprite mBackground;
};

#endif