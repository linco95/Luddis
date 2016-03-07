#include "Button.h"
#include "ResourceManager.h"
#include "GUIManager.h"
#include "EventManager.h"
#include "ViewUtility.h"
#include "VectorMath.h"
#include <vector>

static const std::string DEFAULT_FONTTYPE = "Resources/Fonts/Arial.ttf";
//GLOBAL_CLICK prevents more than one button from activating per loop.
static bool GLOBAL_CLICK = false;

Button::Button(std::string graphicFilename, std::string buttonText, std::string buttonFunc, sf::RenderWindow* window, EventManager* eventManager, sf::Vector2f pos, InterfaceElement* owner, ButtonType buttonType, Strata strata) :
	mWindow(window),
	mEventManager(eventManager),
	mButtonType(buttonType),
	mStrata(strata),
	mIsAlive(true),
	mIsActive(false),
	mClick(false),
	mOwner(owner),
	mButtonFunc(buttonFunc),
	mButtonText(buttonText, ResourceManager::getInstance().getFont(DEFAULT_FONTTYPE), 16),
	mSprite(ResourceManager::getInstance().getTexture(graphicFilename)) {
	setPosition(pos);
	//Set the texture rect to a third of the original image,
	//and set two more rects to correspond to the other
	//"states" of the button, i.e. mouseover etc.
	sf::IntRect spriteRect(mSprite.getTextureRect());
	spriteRect.width /= 3;
	mSprite.setTextureRect(spriteRect);
	for (int i = 0; i < 3; i++) {
		mRects[i] = spriteRect;
		mRects[i].left += i*spriteRect.width;
	}
	sf::FloatRect textRect = mButtonText.getGlobalBounds();
	float xScale = (float)textRect.width / spriteRect.width;
	if ((float)textRect.width + 5.0f> spriteRect.width) {
		mSprite.setScale(xScale + 0.5f, 1);
	}

	mButtonText.setColor(sf::Color::Black);
	mButtonText.setOrigin(textRect.width / 2, textRect.height / 2);
	mSprite.setOrigin((float)spriteRect.width / 2, (float)spriteRect.height / 2);
	mEventManager->attatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed, sf::Event::MouseMoved });

#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	//Debug info
	sf::Vector2f rectSize((float)spriteRect.width, (float)spriteRect.height);
	mDebugCircle.setPointCount(15);
	mDebugCircle.setRadius(rectSize.x / 2);
	mDebugCircle.setOrigin(rectSize / 2.0f);
	mDebugCircle.setFillColor(sf::Color(0, 255, 0, 120));
	mDebugCircle.setOutlineColor(sf::Color(0, 255, 0, 120));

	mDebugRect.setSize(rectSize);
	mDebugRect.setOrigin(rectSize / 2.0f);
	mDebugRect.setFillColor(sf::Color(0, 255, 0, 120));
	mDebugRect.setOutlineColor(sf::Color(0, 255, 0, 120));
#endif
}

Button::~Button() {
	mEventManager->detatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed, sf::Event::MouseMoved });
}

void Button::tick(const sf::Time& deltaTime) {
	GLOBAL_CLICK = false;
}

bool Button::isActive() const {
	return mIsActive;
}

void Button::setActive(const bool& active) {
	mIsActive = active;
}

Button::Strata Button::getRenderLayer() const {
	return mStrata;
}

void Button::setStrata(Strata strata){
	mStrata = strata;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mButtonText, states);

#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	if (mButtonType == CIRCLE)
		target.draw(mDebugCircle, states);
	else
		target.draw(mDebugRect, states);
#endif
}

bool Button::isAlive() const {
	return mIsAlive;
}

void Button::onEvent(const sf::Event &aEvent) {
	if (mIsActive) {
		//The viewport has to be changed here, since all EventObservers are iterated in the same vector.
		sf::View GUIView(ViewUtility::getViewSize());
		sf::View mapView = mWindow->getView();
		mWindow->setView(GUIView);
		static sf::Vector2f mousePos(0, 0);
		//Listen for mouse clicks
		sf::FloatRect rect(mSprite.getGlobalBounds());
		switch (mButtonType) {
		case RECTANGLE:
			if (aEvent.type == sf::Event::MouseButtonPressed&&
				aEvent.mouseButton.button == sf::Mouse::Left) {
				mousePos = mWindow->mapPixelToCoords(sf::Vector2i(aEvent.mouseButton.x, aEvent.mouseButton.y)) - getPosition();

				//Check to see if the mouse position is within the images bounds
				if (rect.contains(mousePos)) {
					mSprite.setTextureRect(mRects[2]);
					mClick = true;
				}
			}
			//If the mouse is released within the bounds of the image,
			//call the function of the owner object corresponding
			//to the string passed on during inception
			else if (aEvent.type == sf::Event::MouseButtonReleased&&
				aEvent.mouseButton.button == sf::Mouse::Left) {
				mousePos = mWindow->mapPixelToCoords(sf::Vector2i(aEvent.mouseButton.x, aEvent.mouseButton.y)) - getPosition();

				if (rect.contains(mousePos) && GLOBAL_CLICK == false && mClick == true) {
					GLOBAL_CLICK = true;
					mOwner->onClick(mButtonFunc);
				}
			}
			else if (aEvent.type == sf::Event::MouseMoved) {
				mousePos = mWindow->mapPixelToCoords(sf::Vector2i(aEvent.mouseMove.x, aEvent.mouseMove.y)) - getPosition();

				if (mSprite.getGlobalBounds().contains(mousePos)) {
					mSprite.setTextureRect(mRects[1]);
				}
				else {
					mSprite.setTextureRect(mRects[0]);
					mClick = false;
				}
			}
			break;

		case CIRCLE:
			if (aEvent.type == sf::Event::MouseButtonPressed&&
				aEvent.mouseButton.button == sf::Mouse::Left) {
				mousePos = mWindow->mapPixelToCoords(sf::Vector2i(aEvent.mouseButton.x, aEvent.mouseButton.y)) - getPosition();
				float distance = VectorMath::getVectorLength(mSprite.getPosition() - mousePos);

				//Check to see if the mouse position is within the images bounds
				if (distance <= mSprite.getGlobalBounds().height / 2) {
					mSprite.setTextureRect(mRects[2]);
					mClick = true;
				}
			}
			//If the mouse is released within the bounds of the image,
			//call the function of the owner object corresponding
			//to the string passed on during inception
			else if (aEvent.type == sf::Event::MouseButtonReleased&&
				aEvent.mouseButton.button == sf::Mouse::Left) {
				mousePos = mWindow->mapPixelToCoords(sf::Vector2i(aEvent.mouseButton.x, aEvent.mouseButton.y)) - getPosition();
				float distance = VectorMath::getVectorLength(mSprite.getPosition() - mousePos);
				if (distance <= mSprite.getGlobalBounds().height / 2 && GLOBAL_CLICK == false && mClick == true) {
					GLOBAL_CLICK = true;
					mOwner->onClick(mButtonFunc);
				}
			}
			else if (aEvent.type == sf::Event::MouseMoved) {
				mousePos = mWindow->mapPixelToCoords(sf::Vector2i(aEvent.mouseMove.x, aEvent.mouseMove.y)) - getPosition();
				float distance = VectorMath::getVectorLength(mSprite.getPosition() - mousePos);
				if (distance <= mSprite.getGlobalBounds().height / 2) {
					mSprite.setTextureRect(mRects[1]);
				}
				else {
					mSprite.setTextureRect(mRects[0]);
					mClick = false;
				}
			}
		}
		//Reset view afterwards
		mWindow->setView(mapView);
	}
}

void Button::kill() {
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