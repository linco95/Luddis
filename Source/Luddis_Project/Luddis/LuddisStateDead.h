#ifndef _INCLUDED_LUDDISSTATEDEAD_
#define _INCLUDED_LUDDISSTATEDEAD_

#include "LuddisState.h"

class Luddis;

class LuddisStateDead : public LuddisState {
public:
	LuddisStateDead(Luddis* playerPtr);
	~LuddisStateDead();

	void tick(const sf::Time& deltaTime) override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;

private:
	Luddis* mPlayerPtr;
};

#endif // !_INCLUDED_LUDDISDEADSTATE_
