#include "Button.h"
#include "ResourceManager.h"

Button::Button(std::string graphicFilename, sf::RenderWindow* window, sf::Vector2f pos, void (*action)(int, double)) :
mWindow(window),
mClicked(false),
mIsAlive(true),
mIsActive(true),
mSprite(ResourceManager::getInstance().getTexture(graphicFilename)){
	sf::IntRect rect(mSprite.getTextureRect());
	rect.width  /= 3;
	mSprite.setTextureRect(rect);
	for (int i = 0; i < 3; i++){
		mRects[i] = rect;
		mRects[i].left += i*rect.width;
	}
	mSprite.setPosition(pos);
}

Button::~Button(){

}

void Button::tick(const sf::Time& deltaTime){
	updateInput();
}

bool Button::isAlive() const{
	return mIsAlive;
}

bool Button::isActive() const{
	return mIsActive;
}

void Button::setActive(const bool& active){
	mIsActive = active;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(mSprite, states);
}

void Button::setButtonPosition(sf::Vector2f pos){
	mSprite.setPosition(pos);
}

void Button::updateInput(){
	sf::Vector2f vector = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
	if (mClicked &&
		!sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		mSprite.getGlobalBounds().contains(vector)){
		//Release click
		onClick();
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

Button::RenderLayer Button::getRenderLayer() const{
	return FOREGROUND;
}

void Button::onClick(){
	//Take action

}