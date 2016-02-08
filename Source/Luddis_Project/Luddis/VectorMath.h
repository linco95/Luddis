#ifndef _INCLUDED_VECTORMATH_H_
#define _INCLUDED_VECTORMATH_H_

#include <SFML/System/Vector2.hpp>


struct VectorMath{

	VectorMath() = delete;
	~VectorMath();
	VectorMath& operator=(const VectorMath&) = delete;
	VectorMath(const VectorMath&) = delete;
	static float dotProduct(const  sf::Vector2f& a, const sf::Vector2f& b);
	static float getVectorLength(const sf::Vector2f& vector);
	static float getVectorLengthSq(const sf::Vector2f& vector);
	static sf::Vector2f normalizeVector(const sf::Vector2f& vector);
	static float getAngle(const sf::Vector2f& v1, const sf::Vector2f& v2);
	static sf::Vector2f rotateVector(const sf::Vector2f& vector, const float& degrees);
	static sf::Vector2f projectVector(const sf::Vector2f& aVector, const sf::Vector2f& aProjectOn);

};

#endif // !_INCLUDED_VECTORMATH_H_
