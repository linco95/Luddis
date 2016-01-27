#ifndef INCLUDED_LEVEL
#define INCLUDED_LEVEL

#include <SFML\Graphics.hpp>

class Level : public sf::Drawable {

public:

	Level();
	~Level();


	void initializeLevel();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// "/resources/level1BG.png"
private:

	sf::View mView;
	sf::Sprite mBackground;
};

#endif