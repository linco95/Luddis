#include "Button.h"
#include "ResourceManager.h"
#include <iostream>

static const std::string DEFAULT_FONTTYPE = "resources/fonts/arial.ttf";

Button::Button(std::string graphicFilename, std::string buttonText, sf::RenderWindow* window, sf::Vector2f pos, void(*action)()) :
mWindow(window),
mIsAlive(true),
mClicked(false),
mIsActive(false),
mButtonText(buttonText, ResourceManager::getInstance().getFont(DEFAULT_FONTTYPE), 16),
mSprite(ResourceManager::getInstance().getTexture(graphicFilename)){
	setPosition(pos);
	sf::IntRect rect(mSprite.getTextureRect());
	rect.width  /= 3;
	mSprite.setTextureRect(rect);
	for (int i = 0; i < 3; i++){
		mRects[i] = rect;
		mRects[i].left += i*rect.width;
	}
	mButtonText.setColor(sf::Color::Black);
	mButtonText.setOrigin(mButtonText.getGlobalBounds().width / 2, mButtonText.getGlobalBounds().height / 2);
	mSprite.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
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
		//onClick();
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

void Button::onClick(void(*action)()){
	//Take action
	if (mClicked = true){
		action();
	}
}