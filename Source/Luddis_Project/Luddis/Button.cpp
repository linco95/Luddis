#include "Button.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include "ViewUtility.h"
#include <iostream>
#include <vector>	

static const std::string DEFAULT_FONTTYPE = "resources/fonts/arial.ttf";
static bool CLICKED = false;

Button::Button(std::string graphicFilename, std::string buttonText, std::string buttonFunc, sf::RenderWindow* window, sf::Vector2f pos, InterfaceElement* owner) :
mWindow(window),
mIsAlive(true),
mIsActive(false),
mOwner(owner),
mButtonFunc(buttonFunc),
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
	EventManager::getInstance().attatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed });
#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	mDebugCircle.setPointCount(6);
	mDebugCircle.setRadius(15.0f);
	mDebugRect.setSize((sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height)));
	mDebugRect.setOrigin(mDebugRect.getSize().x/2, mDebugRect.getSize().y/2);
	mDebugCircle.setFillColor(sf::Color(0, 255, 0, 120));
	mDebugCircle.setOutlineColor(sf::Color(0, 255, 0, 120));
	mDebugRect.setFillColor(sf::Color(0, 255, 0, 120));
#endif
}

Button::~Button(){
	EventManager::getInstance().detatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed });
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
	target.draw(mDebugCircle, states);
	target.draw(mDebugRect, states);
#endif
}

bool Button::isAlive() const{
	return mIsAlive;
}

void Button::updateInput(){
	static sf::Vector2f vector(0,0);
	vector = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow)) - getPosition();

#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	//mDebugCircle.setPosition(vector);
#endif

	if (mSprite.getGlobalBounds().contains(vector)){
		//Default
		mSprite.setTextureRect(mRects[1]);
	}
	else{
		//Mouse over
		mSprite.setTextureRect(mRects[0]);
	}
}

void Button::update(const sf::Event &aEvent){
	if (mIsActive){
		//The viewport has to be changed here, since all EventObservers are iterated in the same vector.
		sf::View GUIView(ViewUtility::getViewSize());
		sf::View mapView = mWindow->getView();
		mWindow->setView(GUIView);
		static sf::Vector2f mousePos(0, 0);
		mousePos = mWindow->mapPixelToCoords(sf::Vector2i(aEvent.mouseButton.x, aEvent.mouseButton.y)) - getPosition();
		if (aEvent.type == sf::Event::MouseButtonPressed&&
			aEvent.mouseButton.button == sf::Mouse::Left){

#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
			mDebugCircle.setPosition(mousePos);
#endif
			if (mSprite.getGlobalBounds().contains(mousePos)){
				mSprite.setTextureRect(mRects[2]);
			}
		}
		else if (aEvent.type == sf::Event::MouseButtonReleased&&
			aEvent.mouseButton.button == sf::Mouse::Left){
			sf::FloatRect rect(mSprite.getGlobalBounds());
			if (rect.contains(mousePos) && CLICKED == false){
				CLICKED = true;
				mOwner->onClick(mButtonFunc);
			}
		}
		mWindow->setView(mapView);
	}
}

void Button::kill(){
	mIsAlive = false;
}