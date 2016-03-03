#include "EventZone.h"
#include "GameStateLevel.h"

static const std::string SPIDER_DIALOGUE = "Resources/Configs/Dialogue/SpiderDialogue";

EventZone::EventZone(EventType eventType, sf::Vector2f pos, sf::Shape* shape, float rotation, int level) :
mIsActive(true),
mIsAlive(true),
mEventType(eventType),
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
	if (collidable->getCollisionCategory() == PLAYER_OBJECT){
		mIsAlive = false;
		std::string filename = SPIDER_DIALOGUE;
		switch (mEventType)
		{
		case SPIDER:
			filename += std::to_string(mLevel) + ".json";
			mGameStateLevel->createDialogue(filename);
			break;

		case SPIDER_1_END:
			filename += std::to_string(mLevel) + "end1.json";
			mGameStateLevel->createDialogue(filename);
			break;

		case SPIDER_2_END:
			filename += std::to_string(mLevel) + "end2.json";
			mGameStateLevel->createDialogue(filename);
			break;

		case SPIDER_3_END:
			//TODO: Add some kind of check to see if the mission has accually been completed
			filename += std::to_string(mLevel) + "end3.json";
			mGameStateLevel->createDialogue(filename);
			break;
		}
	}
}

void EventZone::stun(const sf::Time& deltatime) {
	return;
}