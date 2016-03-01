#include "Polynomial.h"

Polynomial::Polynomial(){

}

Polynomial::~Polynomial(){

}

void Polynomial::addTerm(float k, float exponent){
	Term term{ k, exponent };
	mPolynamial.push_back(term);
}

const Polynomial::PolynamialExpression Polynomial::getPolynonialExpression()
{
	return mPolynamial;
}
