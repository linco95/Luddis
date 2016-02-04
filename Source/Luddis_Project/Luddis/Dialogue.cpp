#include "Dialogue.h"
#include "ResourceManager.h"

static const std::string FONTTYPE = "Resources/Fonts/arial.ttf";

Dialogue::Dialogue(std::string text, sf::RenderWindow* window):
mButtonCount(0),
mWindow(window),
mDialogueText(text, ResourceManager::getInstance().getFont(FONTTYPE)){
	mDialogueText.setPosition(sf::Vector2f(100, 100));
}

Dialogue::~Dialogue(){
	for (int i = 0; i < mButtonCount; i++){
		delete mButtons[i];
	}
}

void Dialogue::tick(const sf::Time& deltaTime){
	updateText(deltaTime);
}

void Dialogue::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mDialogueText, states);
	for (int i = 0; i < mButtonCount; i++){
		target.draw(*mButtons[i], states);
	}
}

bool Dialogue::isAlive(){
	return mIsAlive;
}

Dialogue::RenderLayer Dialogue::getRenderLayer() const{
	return GUI;
}

void Dialogue::updateText(const sf::Time& deltaTime){
	
}

void Dialogue::addButton(std::string buttonFile, sf::Vector2f pos){
	if (mButtonCount < 4){
		mButtonCount++;
		mButtons[mButtonCount] = new Button(buttonFile, mWindow, pos);
	}
}