#ifndef INCLUDED_POWERUPS
#define INCLUDED_POWERUPS

#include <SFML\System.hpp>

class PowerUps {
public:
	PowerUps();
	~PowerUps();

	virtual void activate(sf::Time deltaTime) = 0;
};

#endif // !INCLUDED_POWERUPS