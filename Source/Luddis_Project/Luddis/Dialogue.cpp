#include "Dialogue.h"

Dialogue::Dialogue(std::string textfile){
	
}

Dialogue::~Dialogue(){

}

void Dialogue::tick(const sf::Time& deltaTime){
	updateText(deltaTime);
}

void Dialogue::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mDialogueText, states);
}

bool Dialogue::isAlive(){
	return mIsAlive;
}

Dialogue::RenderLayer Dialogue::getRenderLayer() const{
	return GUI;
}

void Dialogue::updateText(const sf::Time& deltaTime){

}
