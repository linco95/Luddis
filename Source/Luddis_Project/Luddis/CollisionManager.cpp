#include "CollisionManager.h"
#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Shape.hpp>
#endif
#include "VectorMath.h"
#include "Debug.h"
#include <cassert>

static const int COLLISIONRADIUS = 600;

CollisionManager::CollisionManager() :
	mCollidables() {
}

CollisionManager::~CollisionManager() {
	emptyVector();
}

CollisionManager& CollisionManager::getInstance() {
	static CollisionManager cM;
	return cM;
}

void CollisionManager::addCollidable(CollidableEntity* collidable) {
	mCollidables.push_back(collidable);
}

void CollisionManager::removeDeadCollidables() {
	CollidableVector temp;
	for (auto e : mCollidables) {
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
	for (auto c : mCollidables) {
		// Get the hitboxshape
		sf::Shape* shape = c->getNarrowHitbox();
		assert(shape != 0);

		// Set the color that should be drawn
		shape->setFillColor(sf::Color(0, 255, 0, 155));
		// Set an outline for the shape
		shape->setOutlineColor(sf::Color::Black);
		shape->setOutlineThickness(0.0f);

		// create a pivot dot to localize the center
		const int PIVOTSIZE = 4;
		sf::CircleShape pivot(2, PIVOTSIZE);
		pivot.setFillColor(sf::Color::Black);
		pivot.setPosition(shape->getPosition() - sf::Vector2f(PIVOTSIZE / 2, PIVOTSIZE / 2));

		for (auto d : mDebugPoints) {
			aWindow.draw(d);
		}
		mDebugPoints.empty();
		for (auto d : mDebugAxes) {
			aWindow.draw(d);
		}
		mDebugAxes.empty();

		aWindow.draw(*shape);
		aWindow.draw(pivot);
	}
}
#endif

#pragma region Narrow Collision

/**
* Projects the shape on the vector, and returns the min- respective max-values of the coefficient to the axis vector
* returns a pair with: first = min, second = max
*/
std::pair<float, float> getProjection(const sf::Shape& shape, const sf::Vector2f& axis) {
	sf::Transform trans = shape.getTransform();

	float minVal = VectorMath::dotProduct(axis, VectorMath::projectVector(trans.transformPoint(shape.getPoint(0)), axis));
	float maxVal = minVal;
		
	for (std::size_t i = 1; i < shape.getPointCount(); i++) {
		// Get the position vector to the i:th point on the shape (Taking the origin into account)
		float temp = VectorMath::dotProduct(axis, VectorMath::projectVector(trans.transformPoint(shape.getPoint(i)), axis));
		// Project that point on the axis (Formula: (axis * vecToPoint) / (axis * axis)) (intentionally leaving out multiplying in the vector we're projecting on to get the coefficient)
		// Initialize the min and max values if this is the first iteration of the loop
			// If the current value is less than the smallest value, replace the smallest value
		if (temp < minVal) {
			minVal = temp;
		}
		// If the current value is greater than the largest value, replace the largest value
		else if (temp > maxVal) {
			maxVal = temp;
		}

	}
	assert(minVal <= maxVal);
	return std::make_pair(minVal, maxVal);
}
#include <iostream>
// Narrow collision phase, using the "Separating Axis Theorem"

void CollisionManager::narrowCollision(std::pair<CollidableEntity*, CollidableEntity*>& colliding) {
	//Do some culling of collision categories to reduce unneccessary calculations
	//Ugly as fuck...
	

	/*
	Here we're getting every side in the first shape. Then we get the normal of that side and project every point in each shape on that vector.
	Then we check if there's a gap between the projected "shadows". If there is a gap, it is impossible for the shapes to be colliding, i.e. just continue to the next shape.
	If we loop through all and find no gap, they are colliding, and we inform each CollidableEntity that the collision happened.
	(This is called the "Separating Axis Theorem")
	ADDITION:
	We're now also saving down the axis with the smallest "shadow overlap". If we then move the shape so that the smallest shadow no longer overlaps, the objects
	are no longer colliding.
	We then send that axis to the Entities so that they can move away.
	*/

	// Get the top pair of the stack
	auto pair = colliding;
	bool isColliding = true;
	// Get each CollidableEntities hitbox
	auto hitboxPair = std::make_pair(pair.first->getNarrowHitbox(), pair.second->getNarrowHitbox());

	// This is the vector going from the first shape, to the second shape.
	sf::Vector2f smallest(0, 0);
	float overlap = 5000; //Just some high number.

	//First shape check
	std::size_t pointCount = hitboxPair.first->getPointCount();
	for (std::size_t i = 0; i < pointCount; i++) {
		// Get two connected points on the first shape (If i == number of points in the first shape, we get the side from the i:th (last) point of the shape, to the 0:th (first) point) 
		sf::Transform trans = hitboxPair.first->getTransform();
		sf::Vector2f firstVec = hitboxPair.first->getPoint(i + 1 == pointCount ? 0 : i + 1);
		sf::Vector2f secondVec = hitboxPair.first->getPoint(i);
		firstVec = trans.transformPoint(firstVec);
		secondVec = trans.transformPoint(secondVec);

		sf::Vector2f axis = VectorMath::normalizeVector(VectorMath::getNormal(secondVec - firstVec));

//#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
//		sf::CircleShape circle(3.0f, 10);
//		circle.setPosition(secondVec);
//		circle.setOrigin(circle.getGlobalBounds().width/2, circle.getGlobalBounds().height/2);
//		circle.setFillColor(sf::Color::Black);
//		mDebugPoints.push_back(circle);
//
//		sf::Vertex point1(secondVec);
//		sf::Vertex point2(axis);
//		sf::VertexArray line(sf::PrimitiveType::Lines, 2);
//		line.append(point1);
//		line.append(point2);
//		mDebugAxes.push_back(line);
//#endif // LUDDIS_DEBUG_DRAW_HITBOXES

		// Project both shapes on the axis
		std::pair<float, float> shapeProj[2];
		shapeProj[0] = getProjection(*hitboxPair.first, axis);
		shapeProj[1] = getProjection(*hitboxPair.second, axis);

		// Look for a gap between the projections and if there's a gap, the shapes are not colliding, i.e. go on to the next hitbox pair
		if (shapeProj[0].first > shapeProj[1].second || shapeProj[1].first > shapeProj[0].second) {
			return;
		}
		else {
			if (shapeProj[0].first < shapeProj[1].second && shapeProj[0].first > shapeProj[1].first) {
				float o = shapeProj[1].second - shapeProj[0].first;
				if (o < overlap) {
					overlap = o;
					smallest = axis;
				}
			}
			// Is first.max > second.min but still within (first.max < second.max)?
			else if (shapeProj[0].second > shapeProj[1].first && shapeProj[0].second < shapeProj[1].second) {
				float o = shapeProj[0].second - shapeProj[1].first;
				if (o < overlap) {
					overlap = o;
					smallest = -axis;
				}
			}
		}
	}

	//Second shape check
	pointCount = hitboxPair.second->getPointCount();
	for (std::size_t i = 0; i < pointCount; i++) {
		// Get two connected points on the first shape (If i == number of points in the first shape, we get the side from the i:th (last) point of the shape, to the 0:th (first) point) 
		sf::Transform trans = hitboxPair.second->getTransform();
		sf::Vector2f firstVec = hitboxPair.second->getPoint(i + 1 == pointCount ? 0 : i + 1);
		sf::Vector2f secondVec = hitboxPair.second->getPoint(i);
		firstVec = trans.transformPoint(firstVec);
		secondVec = trans.transformPoint(secondVec);

		sf::Vector2f axis = VectorMath::normalizeVector(VectorMath::getNormal(secondVec - firstVec));

//#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
//		sf::CircleShape circle(3.0f, 10);
//		circle.setPosition(secondVec);
//		circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
//		circle.setFillColor(sf::Color::Black);
//		mDebugPoints.push_back(circle);
//
//		sf::Vertex point1(secondVec);
//		sf::Vertex point2(axis);
//		sf::VertexArray line(sf::PrimitiveType::Lines, 2);
//		line.append(point1);
//		line.append(point2);
//		mDebugAxes.push_back(line);
//#endif // LUDDIS_DEBUG_DRAW_HITBOXES

		// Project both shapes on the axis
		std::pair<float, float> shapeProj[2];
		shapeProj[0] = getProjection(*hitboxPair.first, axis);
		shapeProj[1] = getProjection(*hitboxPair.second, axis);

		// Look for a gap between the projections and if there's a gap, the shapes are not colliding, i.e. go on to the next hitbox pair
		if (shapeProj[0].first >= shapeProj[1].second || shapeProj[1].first >= shapeProj[0].second) {
			return;
		}
		else {
			if (shapeProj[0].first <= shapeProj[1].second && shapeProj[0].first >= shapeProj[1].first) {
				float o = shapeProj[1].second - shapeProj[0].first;
				if (o < overlap) {
					overlap = o;
					smallest = axis;
				}
			}
			// Is first.max > second.min but still within (first.max < second.max)?
			else if (shapeProj[0].second > shapeProj[1].first && shapeProj[0].second < shapeProj[1].second) {
				float o = shapeProj[0].second - shapeProj[1].first;
				if (o < overlap) {
					overlap = o;
					smallest = -axis;
				}
			}
		}
	}

	// If no gaps were found, collide. We give the smallestOverlap vector in the direction that the shape has to move to not be colliding anymore.
	pair.first->collide(pair.second, smallest * overlap);
	pair.second->collide(pair.first, smallest * overlap);
}
#pragma endregion Helper functions handeling the narrow collision using Separating Axis Theorem

void CollisionManager::detectCollisions() {
	std::stack<std::pair<CollidableEntity*, CollidableEntity*>> colliding;
	assert(colliding.empty());
	CollidableVector collidables(mCollidables);
	for (CollidableVector::size_type i = 0; i < collidables.size(); i++) {
		CollidableEntity *collidable0 = collidables.at(i);
		if (!collidable0->isActive()) continue;
		for (CollidableVector::size_type j = i + 1; j < collidables.size(); j++) {
			CollidableEntity *collidable1 = collidables.at(j);
			if (!collidable1->isActive()) continue;
			CollidableEntity::Category catFirst = collidable0->getCollisionCategory();
			CollidableEntity::Category catSecond = collidable1->getCollisionCategory();
			if (
				abs(collidable0->getPosition().x - collidable1->getPosition().x) > collidable0->getHitBox().width / 2 + collidable1->getHitBox().width / 2 ||
				//If either the first or the second is IGNORE, proceed without checking for collisions.
				catFirst == CollidableEntity::IGNORE || catSecond == CollidableEntity::IGNORE ||
				(catFirst == CollidableEntity::SOLID && catSecond == CollidableEntity::SOLID) ||
				(catFirst == CollidableEntity::PLAYER_OBJECT && catSecond == CollidableEntity::PLAYER_PROJECTILE) || 
				(catSecond == CollidableEntity::PLAYER_OBJECT && catFirst == CollidableEntity::PLAYER_PROJECTILE) ||
				(catFirst == CollidableEntity::PLAYER_PROJECTILE && catSecond == CollidableEntity::PLAYER_PROJECTILE) ||
				(catFirst == CollidableEntity::PLAYER_PROJECTILE && catSecond == CollidableEntity::COLLECT) || 
				(catSecond == CollidableEntity::PLAYER_PROJECTILE && catFirst == CollidableEntity::COLLECT) ||
				(catFirst == CollidableEntity::PLAYER_PROJECTILE && catSecond == CollidableEntity::EVENTZONE) || 
				(catSecond == CollidableEntity::PLAYER_PROJECTILE && catFirst == CollidableEntity::EVENTZONE) ||
				catFirst == CollidableEntity::PLAYER_PROJECTILE && catSecond == CollidableEntity::ENEMY_DAMAGE_OBSTACLE ||
				catSecond == CollidableEntity::PLAYER_PROJECTILE && catFirst == CollidableEntity::ENEMY_DAMAGE_OBSTACLE ||
				(catFirst == CollidableEntity::COLLECT && catSecond == CollidableEntity::COLLECT) ||
				(catFirst == CollidableEntity::COLLECT && catSecond == CollidableEntity::ENEMY_DAMAGE) || 
				(catSecond == CollidableEntity::COLLECT && catFirst == CollidableEntity::ENEMY_DAMAGE) ||
				(catFirst == CollidableEntity::COLLECT && catSecond == CollidableEntity::ENEMY_STUN) || 
				(catSecond == CollidableEntity::COLLECT && catFirst == CollidableEntity::ENEMY_STUN) ||
				(catFirst == CollidableEntity::COLLECT && catSecond == CollidableEntity::SOLID) || 
				(catSecond == CollidableEntity::COLLECT && catFirst == CollidableEntity::SOLID) ||
				(catFirst == CollidableEntity::COLLECT && catSecond == CollidableEntity::EVENTZONE) || 
				(catSecond == CollidableEntity::COLLECT && catFirst == CollidableEntity::EVENTZONE) ||
				//No enemies can collide with eachother, this might not be wanted behavior.
				(catFirst == CollidableEntity::ENEMY_DAMAGE && catSecond == CollidableEntity::ENEMY_DAMAGE) ||
				(catFirst == CollidableEntity::ENEMY_DAMAGE && catSecond == CollidableEntity::ENEMY_STUN) || 
				(catSecond == CollidableEntity::ENEMY_DAMAGE && catFirst == CollidableEntity::ENEMY_STUN) ||
				(catFirst == CollidableEntity::ENEMY_DAMAGE && catSecond == CollidableEntity::EVENTZONE) || 
				(catSecond == CollidableEntity::ENEMY_DAMAGE && catFirst == CollidableEntity::EVENTZONE) ||
				(catFirst == CollidableEntity::ENEMY_STUN && catSecond == CollidableEntity::ENEMY_STUN) ||
				(catFirst == CollidableEntity::ENEMY_STUN && catSecond == CollidableEntity::EVENTZONE) || 
				(catSecond == CollidableEntity::ENEMY_STUN && catFirst == CollidableEntity::EVENTZONE) ||
				(catFirst == CollidableEntity::SOLID && catSecond == CollidableEntity::EVENTZONE) || 
				(catSecond == CollidableEntity::SOLID && catFirst == CollidableEntity::EVENTZONE) ||
				(catFirst == CollidableEntity::EVENTZONE && catSecond == CollidableEntity::EVENTZONE))
				continue;
			if (collidable0->getHitBox().intersects(collidable1->getHitBox()) && (collidable0->getCollisionCategory() != collidable1->getCollisionCategory())) {
				colliding.push(std::make_pair(collidable0, collidable1));
			}
		}
	}
	while (!colliding.empty()) {
		narrowCollision(colliding.top());
		colliding.pop();
	}
}

void CollisionManager::emptyVector() {
	while (!mCollidables.empty()) {
		mCollidables.pop_back();
	}
}