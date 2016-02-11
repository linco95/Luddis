#include "Button.h"
#include "ResourceManager.h"
#include <iostream>

static const std::string DEFAULT_FONTTYPE = "resources/fonts/arial.ttf";

Button::Button(std::string graphicFilename, std::string buttonText, sf::RenderWindow* window, sf::Vector2f pos, InterfaceElement* owner) :
mWindow(window),
mIsAlive(true),
mClicked(false),
mIsActive(false),
mOwner(owner),
mButtonText(buttonText, ResourceManager::getInstance().getFont(DEFAULT_FONTTYPE), 16),
mSprite(ResourceManager::getInstance().getTexture(graphicFilename)){
	setPosition(pos);
	sf::IntRect spriteRect(mSprite.getTextureRect());
	spriteRect.width /= 3;
	mSprite.setTextureRect(spriteRect);
	for (int i = 0; i < 3; i++){
		mRects[i] = spriteRect;
		mRects[i].left += i*spriteRect.width;
	}
	sf::FloatRect textRect = mButtonText.getGlobalBounds();
	float xScale = (float)textRect.width/spriteRect.width;
	if ((float)textRect.width> spriteRect.width){
		mSprite.setScale(xScale+0.5f, 1);
	}
	mButtonText.setColor(sf::Color::Black);
	mButtonText.setOrigin(textRect.width / 2, textRect.height / 2);
	mSprite.setOrigin((float)spriteRect.width / 2, (float)spriteRect.height / 2);
}

Button::~Button(){

}

void Button::tick(const sf::Time& deltaTime){
	updateInput();
}

bool Button::isActive() const{
	return mIsActive;
}

void Button::setActive(const bool& active){
	mIsActive = active;
}

Button::RenderLayer Button::getRenderLayer() const {
	return FOREGROUND;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mButtonText, states);
}

bool Button::isAlive() const{
	return mIsAlive;
}

void Button::updateInput(){
	sf::Vector2f vector = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow)) - getPosition();
	
	if (mClicked &&
		!sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		mSprite.getGlobalBounds().contains(vector)){
		//Release click
		mOwner->onClick(mButtonText.getString());
	}
	if (mSprite.getGlobalBounds().contains(vector)){
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			//Click
			mSprite.setTextureRect(mRects[2]);
			mClicked = true;
		}
		else{
			//Mouse over
			mSprite.setTextureRect(mRects[1]);
			mClicked = false;
		}
	}
	else if (!mSprite.getGlobalBounds().contains(vector)){
		//Default
		mSprite.setTextureRect(mRects[0]);
	}
}

void Button::kill(){
	mIsAlive = false;
}