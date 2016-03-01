#include "CinematicMoveToPoint.h"
#include "VectorMath.h"
#include <SFML/Graphics/Transformable.hpp>

static const float GRACEAREA = 15;

CinematicMoveToPoint::CinematicMoveToPoint(sf::Vector2f finalPoint, sf::Transformable* transformable):
mFinished(false),
mTransformable(transformable),
mFinalPoint(finalPoint){

}

CinematicMoveToPoint::~CinematicMoveToPoint(){

}

const sf::Vector2f CinematicMoveToPoint::tick(const sf::Time & deltaTime)
{
	if (VectorMath::getVectorLengthSq(mTransformable->getPosition() - mFinalPoint) < GRACEAREA) {
		return sf::Vector2f(0, 0);
		mFinished = true;
	}
	return mTransformable->getPosition() - mFinalPoint;
}

bool CinematicMoveToPoint::getFinished() const
{
	return mFinished;
}

CinematicSequence * CinematicMoveToPoint::copy() const
{
	return new CinematicMoveToPoint(*this);
}
