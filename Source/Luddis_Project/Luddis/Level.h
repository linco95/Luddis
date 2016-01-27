#ifndef INCLUDED_LEVEL
#define INCLUDED_LEVEL

#include <SFML\Graphics\View.hpp>

class Level {

public:

	Level();
	~Level();


	void initializeLevel();

	// "/resources/level1BG.png"
private:

	sf::View mView;
	
};

#endif