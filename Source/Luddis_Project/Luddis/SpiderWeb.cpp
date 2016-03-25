#include "SpiderWeb.h"
#include "EntityManager.h"
#include "Inventory.h"
#include "Filter.h"
#include "GameStateLevel.h"
#include "SoundEngine.h"

float const PAUSE_TIME = 2.3f;

const char* TEXTURE = "Resources/Images/Powerup_web.png";

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
			Filter* filter = new Filter(PAUSE_TIME, Filter::FADEOUT, TEXTURE);
			GUIManager* gui = &GameStateLevel::getInstance().getGUIManager();
			gui->addInterfaceElement(filter);
			SoundEngine::getInstance().playEvent("event:/Gameplay/Luddis/Skills/Spiderweb");
		}
	}
}