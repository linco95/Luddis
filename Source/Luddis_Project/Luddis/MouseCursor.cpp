#include "MouseCursor.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "ResourceManager.h"
#include <SFML/Graphics/RenderWindow.hpp>

MouseCursor::MouseCursor() {

}

void MouseCursor::initialize(const char* filepath, sf::RenderWindow& window) {
	mSprite.setTexture(ResourceManager::getInstance().loadTexture(filepath));
	mSprite.setScale(0.5f, 0.5f);
	mWindow = &window;
}

void MouseCursor::tick() {
	setPosition(mWindow->mapPixelToCoords(sf::Mouse::getPosition()));
}

void MouseCursor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
}