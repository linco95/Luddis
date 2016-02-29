#ifndef _INCLUDED_CINEMATICSEQUENCE_
#define _INCLUDED_CINEMATICSEQUENCE_

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

class CinematicSequence {
public:

	virtual const sf::Vector2f tick(const sf::Time& deltaTime) = 0;
	virtual bool getFinished() const = 0;
	virtual CinematicSequence* copy() const = 0;
};

#endif // !_INCLUDED_CINEMATICSEQUENCE_
