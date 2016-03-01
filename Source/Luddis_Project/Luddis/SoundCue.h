#ifndef _INCLUDED_SOUNDCUE_
#define _INCLUDED_SOUNDCUE_

#include "CinematicSequence.h"

//Right now this class acts similar to Tweens and Pauses,
//in that it is played when it is at the front of the queue.
//The event played has to be created PRIOR to playing it.
class SoundCue : public CinematicSequence{
public:
	SoundCue(const char* eventName);
	~SoundCue();

	virtual const sf::Vector2f tick(const sf::Time & deltaTime) override;
	virtual bool getFinished() const override;
	virtual CinematicSequence* copy() const override;

private:
	const char* mEventName;
	bool mFinished;
};

#endif // !_INCLUDED_SOUNDCUE_
