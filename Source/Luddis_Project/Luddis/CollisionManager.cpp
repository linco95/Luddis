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
		temp->setFillColor(sf::Color::Green);
		aWindow.draw(*temp);
	}
}
#endif

#pragma region Narrow Collision
#include <iostream>
std::pair<float, float> getProjection(const sf::Shape& shape, const sf::Vector2f& axis){
	// Avoid calculating each loop iteration
	const float axisSq = VectorMath::dotProduct(axis, axis);

	double minFirst = 0, maxFirst = 0/*, minSecond, maxSecond*/;

	for (std::size_t i = 0; i < shape.getPointCount(); i++){
		sf::Vector2f vecToPoint = shape.getPoint(i);
		float scalar = VectorMath::dotProduct(axis, vecToPoint) / axisSq;
		if (scalar < minFirst){
			minFirst = scalar;
			}
		else if (scalar > maxFirst){
			maxFirst = scalar;
		}
	}
	return std::make_pair(1.0f, 1.0f);
	}
void narrowCollision(std::stack<std::pair<CollidableEntity*, CollidableEntity*>>& colliding){
	while (!colliding.empty()){
		auto pair = colliding.top();
		colliding.pop();

		auto firstShape = pair.first->getNarrowHitbox();
		auto secondShape = pair.second->getNarrowHitbox();

		const std::size_t firstPointCount = firstShape->getPointCount();

		for (std::size_t i = 0; i < firstPointCount; i++) {

			// Every side in one of the shapes
			sf::Vector2f side = firstShape->getPoint(i < firstPointCount ? i + 1 : 0) - firstShape->getPoint(i);

			sf::Vector2f axis = VectorMath::rotateVector(side, 90.f);


					
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