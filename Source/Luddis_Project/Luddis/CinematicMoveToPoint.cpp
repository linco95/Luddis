#include "CinematicMoveToPoint.h"
#include "VectorMath.h"
#include <SFML/Graphics/Transformable.hpp>

CinematicMoveToPoint::CinematicMoveToPoint(sf::Vector2f finalPoint):
mFinished(false),
mFinalPoint(finalPoint){

}

CinematicMoveToPoint::~CinematicMoveToPoint(){

}

const sf::Vector2f CinematicMoveToPoint::tick(const sf::Time & deltaTime)
{
	return sf::Vector2f();
}

bool CinematicMoveToPoint::getFinished() const
{
	return mFinished;
}

CinematicSequence * CinematicMoveToPoint::copy() const
{
	return nullptr;
}
