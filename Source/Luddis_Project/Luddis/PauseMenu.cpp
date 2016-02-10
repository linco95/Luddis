#include "PauseMenu.h"



PauseMenu::PauseMenu():
mBackground(){
	
}

PauseMenu::~PauseMenu(){

}

void PauseMenu::tick(const sf::Time& deltaTime){

}

void PauseMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mBackground, states);
}

PauseMenu::RenderLayer PauseMenu::getRenderLayer() const{
	return BACKGROUND;
}

bool PauseMenu::isAlive() const{
	return mIsAlive;
}

bool PauseMenu::isActive() const{
	return mIsActive;
}

void PauseMenu::setActive(const bool& active){
	mIsActive = active;
}

void PauseMenu::onClick(std::string){

}