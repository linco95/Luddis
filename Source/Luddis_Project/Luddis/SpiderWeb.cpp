#include "SpiderWeb.h"

float const PAUSE_TIME = 3.0f;

SpiderWeb::SpiderWeb(EntityManager* entityManager):
mPauseTime(PAUSE_TIME),
mEntityManager(entityManager),
mStunTimer(0),
mStunning(false)
{

}

SpiderWeb::~SpiderWeb() {

}

void SpiderWeb::activate(sf::Time deltaTime) {
	mEntityManager->stunEntities(deltaTime);
}