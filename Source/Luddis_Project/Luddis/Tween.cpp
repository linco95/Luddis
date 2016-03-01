#include "Tween.h"
#include <cmath>

Tween::Tween(Polynomial polynomial, float x, float finalX, bool increase):
mPolynomial(polynomial),
mX(x), mFinalX(finalX),
mIncrease(increase),
mFinished(false){

}

Tween::~Tween(){

}

const sf::Vector2f Tween::tick(const sf::Time & deltaTime){
	float yVal = 0;
	if(mIncrease)
		mX += deltaTime.asSeconds();
	else
		mX -= deltaTime.asSeconds();

	if ((mIncrease&& mX >= mFinalX) || (!mIncrease&& mX <= mFinalX))
		mFinished = true;

	for (auto p : mPolynomial.getPolynonialExpression()) {
	yVal += p.mK*(std::powf(mX, p.mExponent));
	}

	return mPosition = {mX, yVal};
}

bool Tween::getFinished() const{
	return mFinished;
}

CinematicSequence* Tween::copy() const{
	Tween* tween = new Tween(*this);
	return tween;
}
