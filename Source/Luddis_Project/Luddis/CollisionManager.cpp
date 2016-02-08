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
void CollisionManager::detectCollisions(){
	assert(mColliding.empty());
	CollidableVector collidables(mCollidables);
	for (CollidableVector::size_type i = 0; i < collidables.size(); i++){
		CollidableEntity *collidable0 = collidables.at(i);
		for (CollidableVector::size_type j = i + 1; j < collidables.size(); j++){
			CollidableEntity *collidable1 = collidables.at(j);
			if (collidable0->getHitBox().intersects(collidable1->getHitBox()) && (collidable0->getCollisionCategory() != collidable1->getCollisionCategory())){
					collidable0->collide(collidable1);
					collidable1->collide(collidable0);
				//mColliding.push(std::make_pair(collidable0, collidable1));
			}
		}
	}
	if (!mColliding.empty()){
		narrowCollision();
	}
}

void CollisionManager::narrowCollision(){
	while (!mColliding.empty()){
		auto pair = mColliding.top();
		mColliding.pop();

		auto firstShape = pair.first->getNarrowHitbox();
		auto secondShape = pair.second->getNarrowHitbox();

		const std::size_t firstPointCount = firstShape->getPointCount();

		for (std::size_t i = 0; i < firstPointCount; i++) {

			sf::Vector2f side = firstShape->getPoint(i < firstPointCount ? i + 1 : 0) - firstShape->getPoint(i);

			sf::Vector2f axis = VectorMath::rotateVector(side, 90.f);
			sf::Vector2f minFirst, maxFirst, minSecond, maxSecond;
			// TODO transform point position, project both shapes all points on axis to get both shapes shadow on the axis vector. 
			//if they overlap the shapes collide, tell both collidables and return function. (http://www.sevenson.com.au/actionscript/sat/)
			for (std::size_t j = 0; j < firstPointCount; j++){
				sf::Vector2f vecToPoint = firstShape->getPoint(j);
				sf::Vector2f firstProjected = VectorMath::projectVector(vecToPoint, axis);
				if (VectorMath::getVectorLengthSq(firstProjected) > VectorMath::getVectorLengthSq(maxFirst)){
					
				}
			}
		}

	}
}