#include "EventZone.h"
#include "GameStateLevel.h"

static const std::string SPIDER_DIALOGUE = "Resources/Configs/Dialogue/SpiderDialogue";


EventZone::EventZone(GameStateLevel* gameStateLevel, EventType, sf::Vector2f pos, sf::Shape* shape, float rotation, int level) :
mIsActive(true),
mIsAlive(true),
mLevel(level),
mHitbox(shape),
mGameStateLevel(gameStateLevel){
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

EventZone::RenderLayer EventZone::getRenderLayer() const{
	return BACKGROUND;
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

void EventZone::collide(CollidableEntity* collidable){
	if (collidable->getCollisionCategory() == FRIEND){
		mIsAlive = false;
		switch (mEventType)
		{
		case SPIDER:
			std::string filename = SPIDER_DIALOGUE;
			filename.insert(filename.size(), std::to_string(mLevel) + ".json");

			mGameStateLevel->createDialogue(filename);
			break;
		}
	}
}