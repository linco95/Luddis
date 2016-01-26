#include "VectorMath.h"
#include <cmath>

float VectorMath::dotProduct(sf::Vector2f& a, sf::Vector2f& b){
	return a.x * b.x + a.y* b.y;
}

float VectorMath::getVectorLength(sf::Vector2f& vector){
	return sqrt(dotProduct(vector, vector));
}

float VectorMath::getVectorLengthSq(sf::Vector2f& vector){
	return dotProduct(vector, vector);
}

sf::Vector2f VectorMath::normalizeVector(sf::Vector2f& vector){
	float factor = 1 / getVectorLength(vector);
	sf::Vector2f vect(vector.x*factor, vector.y*factor);
	return vect;
}
