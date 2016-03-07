#ifndef _INCLUDED_TWEEN_
#define _INCLUDED_TWEEN_

#include "CinematicSequence.h"
#include "Polynomial.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

class Tween : public CinematicSequence{
public:
	//Create a polynomial with atleast one term.
	//If increase if false, it will count down
	//(you will also have to use a value lower than x).
	//The difference between x and finalX will
	//act as a timer.
	Tween(Polynomial polynomial, float x, float finalX, bool increase = true);
	~Tween();

	//Perhaps best used as a normalized vector
	//in conjunction with the move function.
	const sf::Vector2f tick(const sf::Time& deltaTime) override;
	bool getFinished() const override;
	CinematicSequence* copy() const override;

private:
	Polynomial mPolynomial;
	float mX, mY, mFinalX;
	bool mIncrease;
	bool mFinished;
};

#endif // !_INCLUDED_TWEEN_
