#ifndef _INCLUDED_CINEMATICPAUSE_
#define _INCLUDED_CINEMATICPAUSE_

#include "CinematicSequence.h"

class CinematicPause : public CinematicSequence{
public:
	CinematicPause(float timeInSeconds);
	~CinematicPause();

	const sf::Vector2f tick(const sf::Time & deltaTime) override;
	bool getFinished() const override;
	CinematicSequence* copy() const override;

private:
	float mTimeLeft;
	bool mFinished;
};

#endif // !_INCLUDED_CINEMATICPAUSE_
