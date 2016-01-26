#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "Luddis.h"

static const std::string APPNAME = "Luddis";
static const int WIDTH = 1080;
static const int HEIGHT = 720;

static const std::string TEXTURE_NAME = "Knight.png";
static const std::string FONT_NAME = "arial.ttf";

int main(){
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), APPNAME, sf::Style::Fullscreen);
	
	sf::Time deltaTime;
	Luddis luddis(TEXTURE_NAME, &window);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		luddis.tick(deltaTime);
		window.clear();
		window.draw(luddis);
		window.display();
	}

	return 0;
}