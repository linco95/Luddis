#include "Tween.h"
#include <cmath>

Tween::Tween(Polynomial polynomial, float x, float finalX, bool increase) :
	mPolynomial(polynomial),
	mX(x), mFinalX(finalX),
	mIncrease(increase),
	mFinished(false) {
	/*for (auto p : mPolynomial.getPolynonialExpression()) {
		mY += p.mK*(std::powf(mX, p.mExponent));
	}*/
}

Tween::~Tween(){

}

const sf::Vector2f Tween::tick(const sf::Time & deltaTime) {
	float dX = 0;
	float dY = 0;
	if (mIncrease) {
		dX = mX;
		mX += deltaTime.asSeconds();
	}
	else {
		dX = mX;
		mX -= deltaTime.asSeconds();
	}

	if ((mIncrease&& mX >= mFinalX) || (!mIncrease&& mX <= mFinalX))
		mFinished = true;

	for (auto p : mPolynomial.getPolynonialExpression()) {
		dY += p.mK*(std::powf(dX, p.mExponent));
	}
	//Poorly optimized, shouldn't have to calculate this (again).
	mY = 0;
	for (auto p : mPolynomial.getPolynonialExpression()) {
		mY += p.mK*(std::powf(mX, p.mExponent));
	}

	sf::Vector2f calcPos1(dX, dY);
	sf::Vector2f calcPos2(mX, mY);

	return calcPos2 - calcPos1;
}

bool Tween::getFinished() const {
	return mFinished;
}

CinematicSequence* Tween::copy() const{
	return new Tween(*this);
}
