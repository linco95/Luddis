#include "CollisionManager.h"
#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Shape.hpp>
#endif
#include "VectorMath.h"
#include <cassert>

CollisionManager::CollisionManager() :
mCollidables(){
}

CollisionManager::~CollisionManager(){

}

CollisionManager& CollisionManager::getInstance(){
	static CollisionManager cM;
	return cM;
}

void CollisionManager::addCollidable(CollidableEntity* collidable){
	mCollidables.push_back(collidable);
}

void CollisionManager::removeDeadCollidables(){
	CollidableVector temp;
	for (auto e : mCollidables){
		if (e->isAlive()) {
			temp.push_back(e);
		}
	}
	mCollidables = temp;
	return;
}

#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
#include "SFML\Graphics\CircleShape.hpp"
void CollisionManager::drawHitboxes(sf::RenderWindow& aWindow) const {
	for(auto c : mCollidables){
		sf::Shape* temp = c->getNarrowHitbox();
		assert(temp != 0);
		temp->setFillColor(sf::Color(0,255,0,155));
		aWindow.draw(*temp);
	}
}
#endif

#pragma region Narrow Collision
#include "GameManager.h"
#include <iostream>
// return a pair of scalars, representing the min, respective max value of the shapes projection on the axis. first = min, second = max
std::pair<float, float> getProjection(const sf::Shape& shape, const sf::Vector2f& axis){
	// Avoid calculating each loop iteration
	const float axisSq = VectorMath::dotProduct(axis, axis);
	sf::Transform shapeTrans = shape.getTransform();
	shapeTrans.translate(-shape.getOrigin());

	float minVal = 0, maxVal = 0;

	for (std::size_t i = 0; i < shape.getPointCount(); i++){
		sf::Vector2f vecToPoint = shapeTrans.transformPoint(shape.getPoint(i));
		const float scalar = VectorMath::dotProduct(axis, vecToPoint) / axisSq;
		if (scalar < minVal){
			minVal = scalar;
			}
		else if (scalar > maxVal){
			maxVal = scalar;
		}
	}
	assert(minVal < maxVal);
	return std::make_pair(minVal, maxVal);
}

void narrowCollision(std::stack<std::pair<CollidableEntity*, CollidableEntity*>>& colliding){
	while (!colliding.empty()){
		auto pair = colliding.top();
		colliding.pop();

		sf::Shape* firstShape = pair.first->getNarrowHitbox();
		sf::Shape* secondShape = pair.second->getNarrowHitbox();

		const std::size_t firstPointCount = firstShape->getPointCount();
		bool isColliding = true;
		for (std::size_t i = 0; i < firstPointCount; i++) {
			// Every side in one of the shapes
			sf::Vector2f firstVec = firstShape->getPoint(i < firstPointCount ? i + 1 : 0),
				secondVec = firstShape->getPoint(i);
			// Transform the points
			sf::Transform shapeTrans = firstShape->getTransform();
			shapeTrans.translate(-firstShape->getOrigin());
			firstVec = shapeTrans.transformPoint(firstVec);
			secondVec = shapeTrans.transformPoint(secondVec);
			sf::Vector2f side = firstVec - secondVec;

			// Get the normal to the side
			sf::Vector2f axis = VectorMath::rotateVector(side, 90.f);

			// Project both shapes on the axis
			std::pair<float, float> shapeProj[2];
			shapeProj[0] = getProjection(*firstShape, axis);
			shapeProj[1] = getProjection(*secondShape, axis);

			// Look for a gap between the projections
			bool A = shapeProj[0].first > shapeProj[1].second,
				B = shapeProj[0].second < shapeProj[1].first;
			
			if (A || B){
				isColliding = false;
				break;
			}
		}
		// If no gaps were found, collide. Not like this, always colliding now!
		if (isColliding){
			pair.first->collide(pair.second);
			pair.second->collide(pair.first);
		}
	}
}
#pragma endregion Helper functions handeling the narrow collision using Separating Axis Theorem

void CollisionManager::detectCollisions(){
	std::stack<std::pair<CollidableEntity*, CollidableEntity*>> colliding;
	assert(colliding.empty());
	CollidableVector collidables(mCollidables);
	for (CollidableVector::size_type i = 0; i < collidables.size(); i++){
		CollidableEntity *collidable0 = collidables.at(i);
		for (CollidableVector::size_type j = i + 1; j < collidables.size(); j++){
			CollidableEntity *collidable1 = collidables.at(j);
			if (collidable0->getHitBox().intersects(collidable1->getHitBox()) && (collidable0->getCollisionCategory() != collidable1->getCollisionCategory())){
				/*	collidable0->collide(collidable1);
					collidable1->collide(collidable0);*/
				colliding.push(std::make_pair(collidable0, collidable1));
			}
		}
	}
	if (!colliding.empty()){
		narrowCollision(colliding);
	}
}