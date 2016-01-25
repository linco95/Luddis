#ifndef _INCLUDED_VECTORMATH_H_
#define _INCLUDED_VECTORMATH_H_

#include <SFML/System/Vector2.hpp>


class VectorMath{
public:
	VectorMath() = delete;
	~VectorMath();
	VectorMath& operator=(VectorMath&) = delete;
	VectorMath(const VectorMath&) = delete;
	static double dotProduct(sf::Vector2<double>& a, sf::Vector2<double>& b);
	static double getVectorLength(sf::Vector2<double>& vector);
	static double getVectorLengthSq(sf::Vector2<double>& vector);
	static sf::Vector2<double> normalizeVector(sf::Vector2<double>& vector);

};

#endif // !_INCLUDED_VECTORMATH_H_
