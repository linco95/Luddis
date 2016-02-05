#include "VectorMath.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

float VectorMath::dotProduct(sf::Vector2f& a, sf::Vector2f& b){
	return a.x * b.x + a.y* b.y;
}

float VectorMath::getVectorLength(sf::Vector2f& vector){
	return sqrt(dotProduct(vector, vector));
}

float VectorMath::getVectorLengthSq(sf::Vector2f& vector){
	return dotProduct(vector, vector);
}
sf::Vector2f VectorMath::projectVector(sf::Vector2f& aVector, sf::Vector2f& aProjectOn){
	return (dotProduct(aProjectOn, aVector) / dotProduct(aProjectOn, aProjectOn)) * aProjectOn;
}

sf::Vector2f VectorMath::normalizeVector(sf::Vector2f& vector){
	if (getVectorLengthSq(vector) == 1) return vector;
	float factor = 1 / getVectorLength(vector);
	 return sf::Vector2f(vector.x*factor, vector.y*factor);
}

// Returns the angle between two vectors.
float VectorMath::getAngle(sf::Vector2f& v1, sf::Vector2f& v2){
	sf::Vector2f nv1 = normalizeVector(v1);
	sf::Vector2f nv2 = normalizeVector(v2);
	return std::atan2(nv2.y, nv2.x) - std::atan2(nv1.y, nv1.x);
}

sf::Vector2f VectorMath::rotateVector(sf::Vector2f& vector, float degrees){
	sf::Vector2f newVector;
	float degreesFromRadians = degrees*(float) M_PI/180.0f;
	newVector.x = vector.x* std::cosf(degreesFromRadians) + vector.y*-std::sinf(degreesFromRadians);
	newVector.y = vector.x*std::sinf(degreesFromRadians) + vector.y*std::cosf(degreesFromRadians);
	return newVector;
}