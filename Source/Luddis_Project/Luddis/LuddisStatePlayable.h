#ifndef _INCLUDED_LUDDISSTATEPLAYABLE_
#define _INCLUDED_LUDDISSTATEPLAYABLE_

#include "LuddisState.h"
#include "PowerupDisplay.h"

class Luddis;
class EntityManager;

namespace sf {
	class RenderWindow;
}

class LuddisStatePlayable : public LuddisState {
public:
	LuddisStatePlayable(Luddis* playerPtr, sf::RenderWindow* window, EntityManager* entityManager, PowerupDisplay* display);
	virtual ~LuddisStatePlayable();
	void tick(const sf::Time& deltaTime) override;
	void collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) override;
	//void draw() override;

private:
	sf::Vector2f getVectorMouseToSprite() const;
	void handleInput(const sf::Time& deltaTime);
	void updateMovement(const sf::Time& deltaTime);
	void attack();
	void updateRotation();
	void changeScale();

	Luddis* mPlayerPtr;
	sf::RenderWindow* mWindow;
	EntityManager* mEntityManager;

	sf::Vector2f mDirectionVector;
	sf::Vector2f mPrevPos;
	sf::Vector2f mScale;

	float mProjectileCooldown;
	float mInvincibility;
	bool mIsFlipped;
	
	bool mMoved;

	PowerupDisplay* mDisplay;
};

#endif // !_INCLUDED_LUDDISSTATEPLAYABLE_
