#include "EventZone.h"
#include "Spider.h"
#include "GameStateLevel.h"
#include "Inventory.h"

static const std::string SPIDER_DIALOGUE = "Resources/Configs/Dialogue/SpiderDialogue";

EventZone::EventZone(EventType eventType, sf::Vector2f pos, sf::Shape* shape, float rotation, int level, EntityManager* entityManager) :
mIsActive(true),
mIsAlive(true),
mEventType(eventType),
mEntityManager(entityManager),
mLevel(level),
mHitbox(shape),
mGameStateLevel(&GameStateLevel::getInstance()){
	setPosition(pos);
	if (mHitbox->getPointCount() == 4){
		mCollisionType = REC;
	}
	else{
		mCollisionType = CIRCLE;
	}
	mBigHitbox = mHitbox->getLocalBounds();
}

EventZone::~EventZone(){
	delete mHitbox;
}

void EventZone::tick(const sf::Time& deltaTime){

}

void EventZone::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	
}

bool EventZone::isAlive() const{
	return mIsAlive;
}

bool EventZone::isActive() const{
	return mIsActive;
}

void EventZone::setActive(const bool& active){
	mIsActive = active;
}

Renderer::RenderLayer EventZone::getRenderLayer() const{
	return Renderer::BACKGROUND;
}

sf::FloatRect EventZone::getHitBox(){
	return getTransform().transformRect(mBigHitbox);
}

sf::Shape* EventZone::getNarrowHitbox() const{
	mHitbox->setPosition(getPosition());
	return mHitbox;
}

EventZone::Category EventZone::getCollisionCategory(){
	return EVENTZONE;
}

EventZone::Type EventZone::getCollisionType(){
	return mCollisionType;
}

void EventZone::collide(CollidableEntity* collidable, const sf::Vector2f& moveAway){
	if (collidable->getCollisionCategory() == PLAYER_OBJECT){
		mIsAlive = false;
		std::string filename = SPIDER_DIALOGUE;
		Spider* spider;
		switch (mEventType)
		{
		case SPIDER:
			filename += std::to_string(mLevel) + ".json";
			mGameStateLevel->createDialogue(filename);
			spider = new Spider(mWindow, sf::Vector2f(getPosition().x, 0.0f));
			//mResettableGUI.addInterfaceElement(mSpider);

			break;

		case SPIDER_1_END:
			filename += std::to_string(mLevel) + "end1.json";
			mGameStateLevel->createDialogue(filename);
			break;

		case SPIDER_2_END:
			if (mLevel == 1) {
				if (Inventory::getInstance().getEggs() == 10)
					filename += std::to_string(mLevel) + "end2.json";
				else if (Inventory::getInstance().getEggs() <= 9 && Inventory::getInstance().getEggs() >= 1)
					filename += std::to_string(mLevel) + "end2b.json";
				else if (Inventory::getInstance().getEggs() == 0)
					filename += std::to_string(mLevel) + "end1.json";
			}
			mGameStateLevel->createDialogue(filename);
			break;

		case SPIDER_3_END:
			if (mLevel == 1) {
				if (Inventory::getInstance().getEggs() == 10)
					filename += std::to_string(mLevel) + "end3.json";
				else if (Inventory::getInstance().getEggs() <= 9 && Inventory::getInstance().getEggs() >= 1)
					filename += std::to_string(mLevel) + "end3b.json";
				else if (Inventory::getInstance().getEggs() == 0)
					filename += std::to_string(mLevel) + "end1.json";
			}
			mGameStateLevel->createDialogue(filename);
			break;
		}
	}
}

void EventZone::stun(const sf::Time& deltatime) {
	return;
}