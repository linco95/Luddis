#ifndef _INCLUDED_LUDDISSTATESTUNNED
#define _INCLUDED_LUDDISSTATESTUNNED

#include "LuddisState.h"
#include "PowerupDisplay.h"

class Luddis;
class EntityManager;
namespace sf {
	class RenderWindow;
}

class LuddisStateStunned :public LuddisState {
public:
	LuddisStateStunned(Luddis* playerPtr, float stunDuration, sf::RenderWindow* window, EntityManager* entityManager, PowerupDisplay* display);
	virtual ~LuddisStateStunned();
	void tick(const sf::Time& deltaTime) override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;

private:
	Luddis* mPlayerPtr;
	sf::RenderWindow* mWindow;
	EntityManager* mEntityManager;

	float mStunDuration;
	float mInvincibility;

	PowerupDisplay* mDisplay;
};

#endif // !_INCLUDED_LUDDISSTATESTUNNED
