#ifndef _INCLUDED_CINEMATICMOVETOPOINT_
#define _INCLUDED_CINEMATICMOVETOPOINT_

#include "CinematicSequence.h"
namespace sf {
	class Transformable;
}
#include <SFML/System/Vector2.hpp>

class CinematicMoveToPoint : public CinematicSequence {
public:
	CinematicMoveToPoint(sf::Vector2f finalPoint);
	~CinematicMoveToPoint();

	const sf::Vector2f tick(const sf::Time & deltaTime) override;
	bool getFinished() const override;
	CinematicSequence * copy() const override;

private:
	bool mFinished;
	sf::Vector2f mFinalPoint;

};

#endif // !_INCLUDED_CINEMATICMOVETOPOINT_
