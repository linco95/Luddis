#include "EventZone.h"
#include "GameStateLevel.h"

static const std::string SPIDER_DIALOGUE = "Resources/Configs/Dialogue/SpiderDialogue";


EventZone::EventZone(GameStateLevel* gameStateLevel, EventType eventType, sf::Vector2f pos, sf::Shape* shape, float rotation, int level) :
mIsActive(true),
mIsAlive(true),
mEventType(eventType),
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

void EventZone::collide(CollidableEntity* collidable, const sf::Vector2f& moveAway){
	if (collidable->getCollisionCategory() == FRIEND){
		mIsAlive = false;
		std::string filename = SPIDER_DIALOGUE;
		switch (mEventType)
		{
		case SPIDER:
			filename += std::to_string(mLevel) + ".json";
			mGameStateLevel->createDialogue(filename);
			break;

		case SPIDER_EASY_END:
			filename += std::to_string(mLevel) + "easyend.json";
			mGameStateLevel->createDialogue(filename);
			break;

		case SPIDER_MEDIUM_END:
			filename += std::to_string(mLevel) + "mediumend.json";
			mGameStateLevel->createDialogue(filename);
			break;

		case SPIDER_HARD_END:
			filename += std::to_string(mLevel) + "hardend.json";
			mGameStateLevel->createDialogue(filename);
			break;
		}
	}
}