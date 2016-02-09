#include "Button.h"
#include "ResourceManager.h"
#include <iostream>

static const std::string DEFAULT_FONTTYPE = "resources/fonts/arial.ttf";

Button::Button(std::string graphicFilename, std::string buttonText, sf::RenderWindow* window, sf::Vector2f pos, void(*action)(int, double)) :
mWindow(window),
mIsAlive(true),
mClicked(false),
mIsActive(true),
mButtonText(buttonText, ResourceManager::getInstance().getFont(DEFAULT_FONTTYPE), 16),
mSprite(ResourceManager::getInstance().getTexture(graphicFilename)){
	sf::IntRect rect(mSprite.getTextureRect());
	rect.width  /= 3;
	mSprite.setTextureRect(rect);
	for (int i = 0; i < 3; i++){
		mRects[i] = rect;
		mRects[i].left += i*rect.width;
	}
	mButtonText.setColor(sf::Color(0, 0, 0));
	setPosition(pos);
	mButtonText.move(mButtonText.getGlobalBounds().width / 2, mButtonText.getGlobalBounds().height / 2);
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

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mButtonText, states);
}

bool Button::isAlive() const{
	return mIsAlive;
}

Button::RenderLayer Button::getRenderLayer() const{
	return GUI_FOREGROUND;
}

void Button::updateInput(){
	sf::Vector2f vector(sf::Mouse::getPosition(*mWindow));
	//Map pixel to coords does not work when the viewport changes
	if (mClicked &&
		!sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		mSprite.getLocalBounds().contains(vector)){
		//Release click
		onClick();
	}
	if (mSprite.getLocalBounds().contains(vector)){
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
	else if (!mSprite.getLocalBounds().contains(vector)){
		//Default
		mSprite.setTextureRect(mRects[0]);
	}
}

void Button::onClick(){
	//Take action

}