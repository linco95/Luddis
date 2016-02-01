#include "GameManager.h"

#include <SFML\Audio.hpp>

using namespace sf;

int main(){

	GameManager::getInstance().run();

	return 0;
}