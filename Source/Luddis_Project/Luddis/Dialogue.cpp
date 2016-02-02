#include "Dialogue.h"
#include "ResourceManager.h"

static const std::string FONTTYPE = "Resources/Fonts/arial.ttf";

Dialogue::Dialogue(std::string text):
mDialogueText(text, ResourceManager::getInstance().getFont(FONTTYPE)){
	
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
