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
		// Get the hitboxshape
		sf::Shape* shape = c->getNarrowHitbox();
		assert(shape != 0);
		
		// Set the color that should be drawn
		shape->setFillColor(sf::Color(0, 255, 0, 155));
		// Set an outline for the shape
		shape->setOutlineColor(sf::Color::Black);
		shape->setOutlineThickness(2.0f);

		// create a pivot dot to localize the center
		const int PIVOTSIZE = 4;
		sf::CircleShape pivot(2, PIVOTSIZE);
		pivot.setFillColor(sf::Color::Black);
		pivot.setPosition(shape->getPosition() - sf::Vector2f(PIVOTSIZE / 2, PIVOTSIZE / 2));

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
std::pair<float, float> getProjection(const sf::Shape& shape, const sf::Vector2f& axis){

	// Avoid calculating this each loop iteration
	const float axisSq = VectorMath::dotProduct(axis, axis);

	// Get the transform of the shape
	sf::Transform shapeTrans = shape.getTransform();
	// Translate the transform to match the shapes' top left corner
	//shapeTrans.translate(-shape.getOrigin());

	float minVal, maxVal;
	bool first = true;
	for (std::size_t i = 0; i < shape.getPointCount(); i++){
		// Get the position vector to the i:th point on the shape (Taking the origin into account)
		sf::Vector2f vecToPoint = shapeTrans.transformPoint(shape.getPoint(i)/* + shape.getOrigin()*/);
		// Project that point on the axis (Formula: (axis * vecToPoint) / (axis * axis)) (intentionally leaving out multiplying in the vector we're projecting on to get the coefficient)
		const float scalar = VectorMath::dotProduct(axis, vecToPoint) / axisSq;
		// Initialize the min and max values if this is the first iteration of the loop
		if (first){
			minVal = maxVal = scalar;
			first = false;
		}
		else{
			// If the current value is less than the smallest value, replace the smallest value
			if (scalar < minVal){
				minVal = scalar;
			}
			// If the current value is greater than the largest value, replace the largest value
			if (scalar > maxVal){
				maxVal = scalar;
			}
		}
	}
	assert(minVal <= maxVal);
	return std::make_pair(minVal, maxVal);
}
#include <iostream>
// Narrow collision phase, using the "Separating Axis Theorem"

bool gapFromShape1to2(sf::Shape* shape1, sf::Shape* shape2, sf::Vector2f& smallestOverlap) {
	// Saving the number of points in the first shape, just to avoid some function calls
	const std::size_t pointCount = shape1->getPointCount();

	// We assume that there has been a collision, until we've proved the opposite
	bool isColliding = true;

	// Create the vector that defines the least change in position that is needed to not collide with the other entity.
	// Initialized to the zero vector, we will check if it is the zero vector, then just assign, don't compare.

	// Loop through all the points in the first shape
	for (std::size_t i = 0; i < pointCount; i++) {

		// Get two connected points on the first shape (If i == number of points in the first shape, we get the side from the i:th (last) point of the shape, to the 0:th (first) point) 
		sf::Vector2f firstVec = shape1->getPoint(i < pointCount ? i + 1 : 0);
		sf::Vector2f secondVec = shape1->getPoint(i);

		// Get the vector that connects two points on the shape ( == one side of the shape)
		sf::Vector2f side = firstVec - secondVec;

		// Get the normal to the side
		sf::Vector2f axis = VectorMath::getNormal(side);

		// Project both shapes on the axis
		std::pair<float, float> shapeProj[2];
		shapeProj[0] = getProjection(*shape1, axis);
		shapeProj[1] = getProjection(*shape2, axis);

		// Look for a gap between the projections and if there's a gap, the shapes are not colliding, i.e. go on to the next hitbox pair
		if (shapeProj[0].first >= shapeProj[1].second || shapeProj[0].second <= shapeProj[1].first) {
			isColliding = false;
			break;
		}
		// If the shape is colliding, but one of the shapes shadows isn't contained inside of the other shape
		else {
			float diff = 0;
			//  Is first.min < second.maxin but still within (first.min > second.min)?
			if (shapeProj[0].first < shapeProj[1].second && shapeProj[0].first > shapeProj[1].first) {
				diff = shapeProj[1].second - shapeProj[0].first;
			}
			// Is first.max > second.min but still within (first.max < second.max)?
			if (shapeProj[0].second > shapeProj[1].first && shapeProj[0].second < shapeProj[1].second) {
				float tempDiff = shapeProj[1].first - shapeProj[0].second;
				// Checks to see if diff is greater than this
				if (abs(diff) > abs(tempDiff))
					diff = tempDiff;
			}
			// If the smallestOverlap hasn't been initialized, or is longer than diff, we set the smallestOverlap to axis* diff
			//std::cout << std::to_string(diff);
			if (VectorMath::getVectorLengthSq(smallestOverlap) == 0 || VectorMath::getVectorLengthSq(smallestOverlap) > diff * diff) {
				// Direction: from first shape -> second shape
				smallestOverlap = axis * diff;
			}
		}
	}
	return isColliding;
}
void narrowCollision(std::stack<std::pair<CollidableEntity*, CollidableEntity*>>& colliding){
	while (!colliding.empty()) {
		// Get the top pair of the stack
		auto pair = colliding.top();
		colliding.pop();

		// Get each CollidableEntities hitbox
		auto hitboxPair = std::make_pair(pair.first->getNarrowHitbox(), pair.second->getNarrowHitbox());

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

		// This is the vector going from the first shape, to the second shape.
		sf::Vector2f smallestOverlap1(0, 0);
		sf::Vector2f smallestOverlap2(0, 0);

		// If no gaps were found, collide. We give the smallestOverlap vector in the direction that the shape has to move to not be colliding anymore.
		if (gapFromShape1to2(hitboxPair.first, hitboxPair.second, smallestOverlap1) && gapFromShape1to2(hitboxPair.second, hitboxPair.first, smallestOverlap2)){
			sf::Vector2f temp = VectorMath::getVectorLengthSq(smallestOverlap1) < VectorMath::getVectorLengthSq(smallestOverlap2) ? smallestOverlap1 : smallestOverlap2;
			/*if (VectorMath::getVectorLengthSq(smallestOverlap) == 0) {
				std::cout << "\a";
			}
			else {*/
				//std::cout << "\a";
				pair.first->collide(pair.second, temp);
				pair.second->collide(pair.first, temp);
			/*}*/
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