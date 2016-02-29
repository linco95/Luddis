#include "LuddisStateCinematic.h"
#include "LuddisStatePlayable.h"
#include "Luddis.h"
#include "EntityManager.h"
#include "VectorMath.h"
#include <SFML/Graphics/RenderWindow.hpp>

static const sf::Vector2f FRONTVECTOR(1, 0);

LuddisStateCinematic::LuddisStateCinematic(float defaultSpeed, Luddis* playerPtr, sf::RenderWindow* window, EntityManager* entityManager):
mPlayerPtr(playerPtr),
mWindow(window),
mEntityManager(entityManager),
mDefaultSpeed(defaultSpeed){

}

LuddisStateCinematic::~LuddisStateCinematic(){
	while (!mSequences.empty()) {
		delete mSequences.front();
		mSequences.pop();
	}
}

void LuddisStateCinematic::tick(const sf::Time & deltaTime){
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
		sf::Vector2f direction = VectorMath::normalizeVector(mSequences.front()->tick(deltaTime));
		float angle = VectorMath::getAngle(FRONTVECTOR, direction);
		mPlayerPtr->setRotation(angle*36);
		if (!mSpeed.empty())
			mPlayerPtr->move(direction*mSpeed.front()*deltaTime.asSeconds());
		else
			mPlayerPtr->move(direction*mDefaultSpeed*deltaTime.asSeconds());
	}
	else
		mPlayerPtr->setPlayerState(new LuddisStatePlayable(mPlayerPtr, mWindow, mEntityManager));
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
