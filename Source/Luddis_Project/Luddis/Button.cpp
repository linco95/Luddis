#include "Button.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include "ViewUtility.h"
#include "VectorMath.h"
#include <vector>
#include "GUIManager.h"

static const std::string DEFAULT_FONTTYPE = "resources/fonts/arial.ttf";
static bool CLICKED = false;

Button::Button(std::string graphicFilename, std::string buttonText, std::string buttonFunc, sf::RenderWindow* window, EventManager* eventManager, sf::Vector2f pos, InterfaceElement* owner, ButtonType buttonType) :
mWindow(window),
mEventManager(eventManager),
mButtonType(buttonType),
mIsAlive(true),
mIsActive(false),
mOwner(owner),
mButtonFunc(buttonFunc),
mButtonText(buttonText, ResourceManager::getInstance().getFont(DEFAULT_FONTTYPE), 16),
mSprite(ResourceManager::getInstance().getTexture(graphicFilename)){
	setPosition(pos);
	//Set the texture rect to a third of the original image,
	//and set two more rects to correspond to the other
	//"states" of the button, i.e. mouseover etc.
	sf::IntRect spriteRect(mSprite.getTextureRect());
	spriteRect.width /= 3;
	mSprite.setTextureRect(spriteRect);
	for (int i = 0; i < 3; i++){
		mRects[i] = spriteRect;
		mRects[i].left += i*spriteRect.width;
	}
	sf::FloatRect textRect = mButtonText.getGlobalBounds();
	float xScale = (float)textRect.width/spriteRect.width;
	if ((float)textRect.width+5.0f> spriteRect.width){
		mSprite.setScale(xScale+0.5f, 1);
	}
	
	mButtonText.setColor(sf::Color::Black);
	mButtonText.setOrigin(textRect.width / 2, textRect.height / 2);
	mSprite.setOrigin((float)spriteRect.width / 2, (float)spriteRect.height / 2);
	mEventManager->attatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed/*, sf::Event::MouseMoved*/ });

#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	//Debug info
	sf::Vector2f rectSize((float)spriteRect.width, (float)spriteRect.height);
	mDebugCircle.setPointCount(15);
	mDebugCircle.setRadius(rectSize.x/2);
	mDebugCircle.setOrigin(rectSize / 2.0f);
	mDebugCircle.setFillColor(sf::Color(0, 255, 0, 120));
	mDebugCircle.setOutlineColor(sf::Color(0, 255, 0, 120));
	//mDebugCircle.setPosition(mSprite.getPosition());
	
	mDebugRect.setSize(rectSize);
	mDebugRect.setOrigin(rectSize / 2.0f);
	mDebugRect.setFillColor(sf::Color(0, 255, 0, 120));
	mDebugRect.setOutlineColor(sf::Color(0, 255, 0, 120));
	//mDebugRect.setPosition(mSprite.getPosition());
#endif
}

Button::~Button(){
	mEventManager->detatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed, sf::Event::MouseMoved});
}

void Button::tick(const sf::Time& deltaTime){
	updateInput();
	CLICKED = false;
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

#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	if(mButtonType==CIRCLE)
		target.draw(mDebugCircle, states);
	else
		target.draw(mDebugRect, states);
#endif
}

bool Button::isAlive() const{
	return mIsAlive;
}

//TODO: move whole function to onEvent
void Button::updateInput(){
	static sf::Vector2f vector(0,0);
	vector = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow)) - getPosition();

	switch(mButtonType) {
	case RECTANGLE:
		if (mSprite.getGlobalBounds().contains(vector)) {
			//Default
			mSprite.setTextureRect(mRects[1]);
		}
		else {
			//Mouse over
			mSprite.setTextureRect(mRects[0]);
		}
		break;

	case CIRCLE:
		float distance = VectorMath::getVectorLength(mSprite.getPosition() - vector);
		if (distance<=mSprite.getGlobalBounds().height/2) {
			//Default
			mSprite.setTextureRect(mRects[1]);
		}
		else {
			//Mouse over
			mSprite.setTextureRect(mRects[0]);
		}
		break;
	}
}

void Button::onEvent(const sf::Event &aEvent){
	if (mIsActive){
		//The viewport has to be changed here, since all EventObservers are iterated in the same vector.
		sf::View GUIView(ViewUtility::getViewSize());
		sf::View mapView = mWindow->getView();
		mWindow->setView(GUIView);
		static sf::Vector2f mousePos(0, 0);
		mousePos = mWindow->mapPixelToCoords(sf::Vector2i(aEvent.mouseButton.x, aEvent.mouseButton.y)) - getPosition();
		//Listen for mouse clicks
		sf::FloatRect rect(mSprite.getGlobalBounds());
		switch (mButtonType) {
		case RECTANGLE:
			if (aEvent.type == sf::Event::MouseButtonPressed&&
				aEvent.mouseButton.button == sf::Mouse::Left) {

				//Check to see if the mouse position is within the images bounds
				if (rect.contains(mousePos)) {
					mSprite.setTextureRect(mRects[2]);
				}
			}
			//If the mouse is released within the bounds of the image,
			//call the function of the owner object corresponding
			//to the string passed on during inception
			else if (aEvent.type == sf::Event::MouseButtonReleased&&
				aEvent.mouseButton.button == sf::Mouse::Left) {
				if (rect.contains(mousePos) && CLICKED == false) {
					CLICKED = true;
					mOwner->onClick(mButtonFunc);
				}
			}
			break;

		case CIRCLE:
			float distance = VectorMath::getVectorLength(mSprite.getPosition() - mousePos);
			if (aEvent.type == sf::Event::MouseButtonPressed&&
				aEvent.mouseButton.button == sf::Mouse::Left) {

				//Check to see if the mouse position is within the images bounds
				if (distance <= mSprite.getGlobalBounds().height / 2) {
					mSprite.setTextureRect(mRects[2]);
				}
			}
			//If the mouse is released within the bounds of the image,
			//call the function of the owner object corresponding
			//to the string passed on during inception
			else if (aEvent.type == sf::Event::MouseButtonReleased&&
				aEvent.mouseButton.button == sf::Mouse::Left) {
					if (distance <= mSprite.getGlobalBounds().height / 2 && CLICKED == false) {
					CLICKED = true;
					mOwner->onClick(mButtonFunc);
				}
			}
		}
		//Reset view afterwards
		mWindow->setView(mapView);
	}
}

void Button::kill(){
	mIsAlive = false;
}

void Button::setScale(sf::Vector2f& scale) {
	mSprite.setScale(scale);
	mButtonText.setScale(scale);
}

void Button::setScale(float x, float y) {
	mSprite.setScale(x, y);
	mButtonText.setScale(x, y);
}