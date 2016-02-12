#ifndef _INCLUDED_GAMESTATE_
#define _INCLUDED_GAMESTATE_

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GameState{
public:
	virtual void update(sf::Clock& clock) = 0;
	virtual void render() = 0;

};

#endif // !_INCLUDED_GAMESTATE_
