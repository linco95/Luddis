#ifndef INCLUDED_SIDERWEB
#define INCLUDED_SIDERWEB

#include "PowerUps.h"
#include "EntityManager.h"
#include "PowerupDisplay.h"

class SpiderWeb :public PowerUps {
public:
	SpiderWeb(EntityManager* entityManager, PowerupDisplay* display);
	~SpiderWeb();

	void activate(sf::Time deltaTime) override;
private:
	float mPauseTime;
	EntityManager* mEntityManager;

	float mStunTimer;
	bool mStunning;
	PowerupDisplay* mDisplay;
};


#endif // !INCLUDED_SIDERWEB
