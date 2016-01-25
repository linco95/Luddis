#include "VectorMath.h"
#include <cmath>

double VectorMath::dotProduct(sf::Vector2<double>& a, sf::Vector2<double>& b){
	return a.x * b.x + a.y* b.y;
}

double VectorMath::getVectorLength(sf::Vector2<double>& vector){
	return sqrt(dotProduct(vector, vector));
}

double VectorMath::getVectorLengthSq(sf::Vector2<double>& vector){
	return dotProduct(vector, vector);
}

sf::Vector2<double> VectorMath::normalizeVector(sf::Vector2<double>& vector){
	double factor = 1 / getVectorLength(vector);
	sf::Vector2<double> vect(vector.x*factor, vector.y*factor);
	return vect;
}
