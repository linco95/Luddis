#ifndef INCLUDED_SIDERWEB
#define INCLUDED_SIDERWEB

#include "PowerUps.h"
#include "EntityManager.h"

class SpiderWeb :public PowerUps {
public:
	SpiderWeb(EntityManager* entityManager);
	~SpiderWeb();

	void activate(sf::Time deltaTime) override;
private:
	float mPauseTime;
	EntityManager* mEntityManager;

	float mStunTimer;
	bool mStunning;
};


#endif // !INCLUDED_SIDERWEB
