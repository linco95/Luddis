#include "Dialogue.h"
#include "ResourceManager.h"
//
static const std::string DIALOGUE_TEXTURE = "Resources/Images/Parchment.png";

Dialogue::Dialogue(std::string text, sf::RenderWindow* window, sf::Vector2f pos):
mButtonCount(0),
mIsAlive(true),
mIsActive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(DIALOGUE_TEXTURE)),
mDialogueText(sf::IntRect(35, 35, 390, 100), text, 24){
	setPosition(pos);
}

Dialogue::~Dialogue(){
	for (int i = 0; i < mButtonCount; i++){
		delete mButtons[i];
	}
}

void Dialogue::tick(const sf::Time& deltaTime){
	updateText(deltaTime);
	for (int i = 0; i < mButtonCount; i++){
		mButtons[i]->tick(deltaTime);
	}
}

void Dialogue::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mDialogueText, states);
	for (int i = 0; i < mButtonCount; i++){
		target.draw(*mButtons[i], states);
	}
}

bool Dialogue::isAlive() const{
	return mIsAlive;
}

bool Dialogue::isActive() const{
	return mIsActive;
}

void Dialogue::setActive(const bool& active){
	mIsActive = active;
}

Dialogue::RenderLayer Dialogue::getRenderLayer() const{
	return GUI;
}

void Dialogue::updateText(const sf::Time& deltaTime){
	//Temp fix
	//mButtonCount = 0;
}

void Dialogue::addButton(std::string buttonFile, sf::Vector2f pos){
	if (mButtonCount < 4){
		mButtonCount++;
		mButtons[mButtonCount-1] = new Button(buttonFile, mWindow, pos);
	}
}