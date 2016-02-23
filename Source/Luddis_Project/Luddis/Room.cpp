#include "Room.h"
#include "ViewUtility.h"
#include "GUIManager.h"
#include "ResourceManager.h"
#include <SFML/Graphics/Rect.hpp>

Room::Room(sf::Vector2f position, GUIManager* guiManager, std::string textureFilename):
mView(ViewUtility::getViewSize()),
mBackground(ResourceManager::getInstance().getTexture(textureFilename)),
mGUIManager(guiManager){
	setPosition(position);
	mView.setCenter(mView.getCenter() + getPosition());
}

Room::~Room() {
	
}

void Room::tick(const sf::Time& deltaTime) {

}

void Room::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mBackground, states);
}

Room::RenderLayer Room::getRenderLayer() const {
	return BACKGROUND;
}

bool Room::isAlive() const {
	return mIsAlive;
}

bool Room::isActive() const{
	return mIsActive;
}

void Room::setActive(const bool& active) {
	mIsActive = active;
}

sf::View Room::getView() const {
	return mView;
}

void Room::kill() {
	mIsAlive = false;
}

void Room::onClick(std::string buttonFunc) {
	std::string levelSubstr = buttonFunc.substr(0, 5);
	std::string roomSubstr = buttonFunc.substr(0, 4);
	if (buttonFunc == "Shop") {

	}
	else if (levelSubstr == "Level") {
		std::string string = buttonFunc.substr(levelSubstr.length(), buttonFunc.length());
		int value = std::stoi(string);
		buttonFuncLevel(value);
	}
	else if (roomSubstr == "Room") {
		std::string string = buttonFunc.substr(roomSubstr.length(), buttonFunc.length());
		int value = std::stoi(string);
		buttonFuncRoom(value);
	}
}

void Room::buttonFuncShop() {

}

void Room::buttonFuncLevel(int level) {

}

void Room::buttonFuncRoom(int room) {

}