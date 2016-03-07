#define _USE_MATH_DEFINES
#include "LuddisStateCinematic.h"
#include "LuddisStatePlayable.h"
#include "Luddis.h"
#include "EntityManager.h"
#include "VectorMath.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "PowerupDisplay.h"

static const sf::Vector2f FRONTVECTOR(1, 0);

LuddisStateCinematic::LuddisStateCinematic(float defaultSpeed, Luddis* playerPtr, sf::RenderWindow* window, EntityManager* entityManager, PowerupDisplay* display):
mPlayerPtr(playerPtr),
mWindow(window),
mEntityManager(entityManager),
mDefaultSpeed(defaultSpeed),
mDisplay(display){

}

LuddisStateCinematic::~LuddisStateCinematic(){
	while (!mSequences.empty()) {
		delete mSequences.front();
		mSequences.pop();
	}
}

void LuddisStateCinematic::tick(const sf::Time & deltaTime){
	if(mSequences.empty())
		return mPlayerPtr->setPlayerState(new LuddisStatePlayable(mPlayerPtr, mWindow, mEntityManager, mDisplay));
	if (mSequences.front()->getFinished()) {
		delete mSequences.front();
		mSequences.pop();
	}

	float value;
	if (!mTimeElapsed.empty() && mTimeElapsed.front() <= 0) {
		mSpeed.pop();
		value = mTimeElapsed.front();
		mTimeElapsed.pop();
		//Subtract the excess time
		if(!mTimeElapsed.empty())
			mTimeElapsed.front() += value;
	}
	else if (!mTimeElapsed.empty()) {
		mTimeElapsed.front() -= deltaTime.asSeconds();
	}

	if (!mSequences.empty()) {
		sf::Vector2f direction = mSequences.front()->tick(deltaTime);
		if (direction == sf::Vector2f(0, 0))
			return;

		direction = VectorMath::normalizeVector(direction);
		if (VectorMath::getVectorLengthSq(direction) != 0) {

			float angle = VectorMath::getAngle(FRONTVECTOR, direction) * 180 / (float)M_PI;
			mPlayerPtr->setRotation(angle);
			if ((direction.x <= 0 && !mIsFlipped) || (direction.x > 0 && mIsFlipped)) {
				mIsFlipped = !mIsFlipped;
			}
		}
		if (!mSpeed.empty())
			mPlayerPtr->move(direction*mSpeed.front()*deltaTime.asSeconds());
		else
			mPlayerPtr->move(direction*mDefaultSpeed*deltaTime.asSeconds());
	}
	else
		mPlayerPtr->setPlayerState(new LuddisStatePlayable(mPlayerPtr, mWindow, mEntityManager, mDisplay));
}

void LuddisStateCinematic::collide(CollidableEntity * collidable, const sf::Vector2f & moveAway){
	//Handle collisions?

}

void LuddisStateCinematic::addCinematicSequence(CinematicSequence* sequence){
	CinematicSequence* seq = sequence->copy();
	mSequences.push(seq);
}

void LuddisStateCinematic::addSpeedShift(float speed, float timer){
	mSpeed.push(speed);
	mTimeElapsed.push(timer);
}
