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
	emptyVector();
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
		temp->setOutlineColor(sf::Color::Black);
		temp->setOutlineThickness(2.0f);
		aWindow.draw(*temp);
		sf::CircleShape pivot(2, 4);
		pivot.setFillColor(sf::Color::Black);
		pivot.setPosition(temp->getPosition() - sf::Vector2f(1,1));
		aWindow.draw(pivot);
	}
}
#endif

#pragma region Narrow Collision
// return a pair of scalars, representing the min, respective max value of the shapes projection on the axis. first = min, second = max
std::pair<float, float> getProjection(const sf::Shape& shape, const sf::Vector2f& axis){
	// Avoid calculating each loop iteration
	const float axisSq = VectorMath::dotProduct(axis, axis);
	sf::Transform shapeTrans = shape.getTransform();
	shapeTrans.translate(-shape.getOrigin());

	float minVal = 0, maxVal = 0;
	bool first = true;
	for (std::size_t i = 0; i < shape.getPointCount(); i++){
		sf::Vector2f vecToPoint = shapeTrans.transformPoint(shape.getPoint(i));
		const float scalar = VectorMath::dotProduct(axis, vecToPoint) / axisSq;
		if (first){
			minVal = maxVal = scalar;
			first = false;
		}
		else{
			if (scalar < minVal){
				minVal = scalar;
			}
			if (scalar > maxVal){
				maxVal = scalar;
			}
		}
	}
	assert(minVal <= maxVal);
	return std::make_pair(minVal, maxVal);
}
#include <iostream>
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
			sf::Vector2f firstVec = firstShape->getPoint(i < firstPointCount ? i + 1 : 0);
			sf::Vector2f secondVec = firstShape->getPoint(i);
			
			// Transform the points
			sf::Transform shapeTrans = firstShape->getTransform();

			////shapeTrans.translate();
			//firstVec = shapeTrans.transformPoint(firstVec);
			//secondVec = shapeTrans.transformPoint(secondVec);
			sf::Vector2f side = firstVec - secondVec;

			// Get the normal to the side
			sf::Vector2f axis = VectorMath::getNormal(side);

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
			//std::cout << '\a';
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
		if (!collidable0->isActive()) continue;
		for (CollidableVector::size_type j = i + 1; j < collidables.size(); j++){
			CollidableEntity *collidable1 = collidables.at(j);
			if (!collidable1->isActive()) continue;
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

void CollisionManager::emptyVector(){
	while (!mCollidables.empty()){
		mCollidables.pop_back();
	}
}