//#include <SFML/Graphics.hpp>

#include "GameManager.h"

int main(){
	
	// TODO: Use gamemanager

	GameManager::getInstance().run();

	//sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!", sf::Style::Fullscreen);
	//while (window.isOpen()) {
	//	sf::Event event;
	//	while (window.pollEvent(event)) {
	//		switch (event.type){
	//		case sf::Event::Closed:
	//			window.close();
	//			break;
	//		case sf::Event::KeyPressed:
	//			if (event.key.code == sf::Keyboard::Escape){
	//			}
	//			break;
	//		}
	//	}
	//	window.clear();
	//	// Draw
	//	
	//	window.display();
	//}

	return 0;
}