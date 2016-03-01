#include "SpiderWeb.h"

float const PAUSE_TIME = 3.0f;

SpiderWeb::SpiderWeb(EntityManager* entityManager, PowerupDisplay* display):
mPauseTime(PAUSE_TIME),
mEntityManager(entityManager),
mStunTimer(0),
mStunning(false),
mDisplay(display)
{

}

SpiderWeb::~SpiderWeb() {

}

void SpiderWeb::activate(sf::Time deltaTime) {
	if (mDisplay->getCooldown() <= 0) {
		mEntityManager->stunEntities(deltaTime);
		mDisplay->activateCooldown();
	}
}