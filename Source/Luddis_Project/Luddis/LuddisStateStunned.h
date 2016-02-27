#ifndef _INCLUDED_LUDDISSTATESTUNNED
#define _INCLUDED_LUDDISSTATESTUNNED

#include "LuddisState.h"

class Luddis;
class EntityManager;
namespace sf {
	class RenderWindow;
}

class LuddisStateStunned :public LuddisState {
public:
	LuddisStateStunned(Luddis* playerPtr, float stunDuration, sf::RenderWindow* window, EntityManager* entityManager);
	virtual ~LuddisStateStunned();
	void tick(const sf::Time& deltaTime) override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;

private:
	Luddis* mPlayerPtr;
	sf::RenderWindow* mWindow;
	EntityManager* mEntityManager;

	float mStunDuration;
	float mInvincibility;
};

#endif // !_INCLUDED_LUDDISSTATESTUNNED
