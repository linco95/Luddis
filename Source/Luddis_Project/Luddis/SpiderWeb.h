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
};


#endif // !INCLUDED_SIDERWEB
