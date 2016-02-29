#ifndef _INCLUDED_POLYNOMIAL_
#define _INCLUDED_POLYNOMIAL_

#include <vector>

class Polynomial {
public:
	struct Term{
		float mK, mExponent;
	};
	typedef std::vector<Term> PolynamialExpression;

	Polynomial();
	~Polynomial();

	//You have to add terms to a polynomial for it to be useful.
	void addTerm(float k, float exponent);
	const PolynamialExpression getPolynonialExpression();

private:
	PolynamialExpression mPolynamial;
	
};

#endif // !_INCLUDED_POLYNOMIAL_
