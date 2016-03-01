#define _USE_MATH_DEFINES
#include "VectorMath.h"
#include "Debug.h"
#include <cmath>
#include <cassert>

float VectorMath::dotProduct(const sf::Vector2f& a, const sf::Vector2f& b){
	return a.x * b.x + a.y * b.y;
}

float VectorMath::getVectorLength(const sf::Vector2f& vector){
	return sqrt(dotProduct(vector, vector));
}

float VectorMath::getVectorLengthSq(const sf::Vector2f& vector){
	return dotProduct(vector, vector);
}
sf::Vector2f VectorMath::projectVector(const sf::Vector2f& aVector, const sf::Vector2f& aProjectOn){
	return (dotProduct(aProjectOn, aVector) / dotProduct(aProjectOn, aProjectOn)) * aProjectOn;
}

sf::Vector2f VectorMath::normalizeVector(const sf::Vector2f& vector){
	if (getVectorLengthSq(vector) == 0) {
		Debug::log("Trying to normalize zero vector.", Debug::WARNING);
		return sf::Vector2f(0, 0);
	}
	//assert(getVectorLengthSq(vector) != 0);
	if (getVectorLengthSq(vector) == 1) return vector;
	float factor = 1 / getVectorLength(vector);
	 return sf::Vector2f(vector.x*factor, vector.y*factor);
}

// Returns the angle between two vectors.
float VectorMath::getAngle(const sf::Vector2f& v1, const sf::Vector2f& v2){
	return std::atan2(v2.y, v2.x) - std::atan2(v1.y, v1.x);
}
sf::Vector2f VectorMath::getNormal(const sf::Vector2f& aVector){
	return sf::Vector2f(-aVector.y, aVector.x);
}

sf::Vector2f VectorMath::rotateVector(const sf::Vector2f& vector, const float& degrees){
	sf::Vector2f newVector;
	float degreesFromRadians = degrees*(float) M_PI/180.0f;
	newVector.x = vector.x* std::cosf(degreesFromRadians) + vector.y*-std::sinf(degreesFromRadians);
	newVector.y = vector.x*std::sinf(degreesFromRadians) + vector.y*std::cosf(degreesFromRadians);
	return newVector;
}