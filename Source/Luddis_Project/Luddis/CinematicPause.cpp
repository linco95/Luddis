#include "CinematicPause.h"

CinematicPause::CinematicPause(float timeInSeconds):
mTimeLeft(timeInSeconds),
mFinished(false){

}

CinematicPause::~CinematicPause(){

}

const sf::Vector2f CinematicPause::tick(const sf::Time & deltaTime){
	mTimeLeft -= deltaTime.asSeconds();
	if (mTimeLeft <= 0)
		mFinished = true;

	return sf::Vector2f(0,0);
}

bool CinematicPause::getFinished() const{
	return mFinished;
}

CinematicSequence* CinematicPause::copy() const{
	
	CinematicPause* cin = new CinematicPause(*this);
	return cin;
}
