#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

static const std::string APPNAME = "Luddis";
static const int WIDTH = 1080;
static const int HEIGHT = 720;

static const std::string TEXTURE_FILE_NAME = "Resources/TestTexture.png";

int main(){
	ResourceManager::getInstance().loadTexture(TEXTURE_FILE_NAME);
	sf::Sprite sprite(ResourceManager::getInstance().getTexture(TEXTURE_FILE_NAME));
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), APPNAME);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}