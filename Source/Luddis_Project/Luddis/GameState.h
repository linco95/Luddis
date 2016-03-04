#ifndef _INCLUDED_GAMESTATE_
#define _INCLUDED_GAMESTATE_

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "EventObserver.h"

class GameState: public EventObserver{
public:
	virtual void update(sf::Clock& clock) = 0;
	virtual void render() = 0;
	virtual void onEvent(const sf::Event &aEvent) = 0;
	virtual void handleEvents() = 0;
	virtual void resetLevel(){}
};

#endif // !_INCLUDED_GAMESTATE_
