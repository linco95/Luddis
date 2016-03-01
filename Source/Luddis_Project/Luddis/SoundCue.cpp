#include "SoundCue.h"
#include "SoundEngine.h"

SoundCue::SoundCue(const char * eventName):
mEventName(eventName),
mFinished(false){

}

SoundCue::~SoundCue(){

}

const sf::Vector2f SoundCue::tick(const sf::Time & deltaTime){
	if (!mFinished) {
		mFinished = false;
		SoundEngine::getInstance().playEvent(mEventName);
	}

	return sf::Vector2f(0, 0);
}

bool SoundCue::getFinished() const{
	return mFinished;
}

CinematicSequence* SoundCue::copy() const{
	return new SoundCue(*this);
}
