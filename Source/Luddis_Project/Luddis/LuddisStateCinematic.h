#ifndef _INCLUDED_LUDDISSTATECINEMATIC_
#define _INCLUDED_LUDDISSTATECINEMATIC_

#include "LuddisState.h"
#include <queue>
#include "Tween.h"
#include "PowerupDisplay.h"

class Luddis;
class EntityManager;
namespace sf {
	class RenderWindow;
}

class LuddisStateCinematic : public LuddisState{
public:
	LuddisStateCinematic(float defaultSpeed, Luddis* playerPtr, sf::RenderWindow* window, EntityManager* entityManager, PowerupDisplay* display, sf::Shape* hitbox);
	~LuddisStateCinematic();

	void tick(const sf::Time& deltaTime) override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;

	void addCinematicSequence(CinematicSequence* sequence);
	void addSpeedShift(float speed, float timer);

private:
	std::queue<CinematicSequence*> mSequences;
	Luddis* mPlayerPtr;
	sf::RenderWindow* mWindow;
	EntityManager* mEntityManager;

	//Used to create different speed intervals
	std::queue<float> mSpeed;
	std::queue<float> mTimeElapsed;
	float mDefaultSpeed;
	PowerupDisplay* mDisplay;
	bool mIsFlipped;
	sf::Shape* mHitbox;
};

#endif // !_INCLUDED_LUDDISSTATECINEMATIC_
