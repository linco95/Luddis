
#ifdef _DEBUG
#include "Debug.h"
#include <iostream>
#include <SFML\Graphics\Shape.hpp>

using namespace std;

void Debug::log(const string& aMsg, const ERRORLEVEL& aLvl){
	cout << "[ERRORLEVEL: " << aLvl << "] " << aMsg << endl;
}

#endif