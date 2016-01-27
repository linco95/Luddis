#ifndef INCLUDED_LEVEL
#define INCLUDED_LEVEL

#include <SFML\Graphics.hpp>

class Level {

public:

	Level();
	~Level();


	void initializeLevel();

	// "/resources/level1BG.png"
private:

	sf::View mView;
	sf::Sprite mBackground;
};

#endif