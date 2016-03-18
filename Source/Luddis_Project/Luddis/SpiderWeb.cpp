#include "SpiderWeb.h"
#include "Inventory.h"
#include "SoundEngine.h"

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
		if (Inventory::getInstance().getDust() > 10){
			Inventory::getInstance().addDust(-10);
			mEntityManager->stunEntities(deltaTime);
			mDisplay->activateCooldown();
			SoundEngine::getInstance().playEvent("event:/Gameplay/Luddis/Skills/Spiderweb");
		}
	}
}