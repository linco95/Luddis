#ifndef _INCLUDED_VECTORMATH_H_
#define _INCLUDED_VECTORMATH_H_

#include <SFML/System/Vector2.hpp>


class VectorMath{
public:
	VectorMath() = delete;
	~VectorMath();
	VectorMath& operator=(VectorMath&) = delete;
	VectorMath(const VectorMath&) = delete;
	static float dotProduct(sf::Vector2f& a, sf::Vector2f& b);
	static float getVectorLength(sf::Vector2f& vector);
	static float getVectorLengthSq(sf::Vector2f& vector);
	static sf::Vector2f normalizeVector(sf::Vector2f& vector);
	static float getAngle(sf::Vector2f& v1, sf::Vector2f& v2);
	static sf::Vector2f rotateVector(sf::Vector2f& vector, float degrees);
};

#endif // !_INCLUDED_VECTORMATH_H_
