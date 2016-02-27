#ifndef _INCLUDED_LUDDISSTATE_
#define _INCLUDED_LUDDISSTATE_

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class CollidableEntity;

class LuddisState {
public:
	virtual void tick(const sf::Time& deltaTime) = 0;
	virtual void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) = 0;
};

#endif // !_INCLUDED_LUDDISSTATE_
