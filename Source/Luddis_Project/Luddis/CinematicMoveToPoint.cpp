#include "CinematicMoveToPoint.h"
#include "VectorMath.h"
#include <SFML/Graphics/Transformable.hpp>

static const float GRACEAREA = 50;

CinematicMoveToPoint::CinematicMoveToPoint(sf::Vector2f finalPoint, sf::Transformable* transformable):
mFinished(false),
mTransformable(transformable),
mFinalPoint(finalPoint){

}

CinematicMoveToPoint::~CinematicMoveToPoint(){

}

const sf::Vector2f CinematicMoveToPoint::tick(const sf::Time & deltaTime)
{
	sf::Vector2f temp2 = mTransformable->getPosition();
	sf::Vector2f temp = mFinalPoint - mTransformable->getPosition();
	if (VectorMath::getVectorLengthSq(temp) < GRACEAREA) {
		mFinished = true;
		return sf::Vector2f(0, 0);
	}
	return temp;
}

bool CinematicMoveToPoint::getFinished() const
{
	return mFinished;
}

CinematicSequence * CinematicMoveToPoint::copy() const
{
	return new CinematicMoveToPoint(*this);
}
