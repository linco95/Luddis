#include "Slider.h"
#include "VectorMath.h"
#include "ViewUtility.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

static const char* FONT = "Resources/Fonts/phitradesign Handwritten-Thin.ttf";

static sf::Vector2f direction = { 1.0f, 0.0f };

Slider::Slider(std::string gaugeFile, std::string sliderFile, float percent, std::string attribute, std::string buttonFunc, sf::Vector2f pos, sf::RenderWindow * window, EventManager * eventManager, InterfaceElement * owner) :
	mGauge(ResourceManager::getInstance().getTexture(gaugeFile)),
	mSlider(ResourceManager::getInstance().getTexture(sliderFile)),
	mIsActive(false),
	mIsAlive(true),
	mIsClicked(false),
	mAttributeText("", ResourceManager::getInstance().getFont(FONT)),
	mAttribute(attribute),
	mButtonFunc(buttonFunc),
	mOwner(owner),
	mEventManager(eventManager),
	mWindow(window) {

	setPosition(pos);
	sf::Vector2f gaugeOrigin(mGauge.getTextureRect().width / 2.0f, mGauge.getTextureRect().height / 2.0f);
	mGauge.setOrigin(gaugeOrigin);

	sf::Vector2f sliderOrigin(mSlider.getTextureRect().width / 2.0f, mSlider.getTextureRect().height / 2.0f);
	mSlider.setOrigin(sliderOrigin);

	mXMinPos = -gaugeOrigin.x + sliderOrigin.x;
	mXMaxPos = gaugeOrigin.x - sliderOrigin.x;

	mYMinPos = -gaugeOrigin.y + sliderOrigin.y;
	mYMaxPos = gaugeOrigin.y - sliderOrigin.y;

	sf::Vector2f maxPos(mXMaxPos, mYMaxPos);
	sf::Vector2f minPos(mXMinPos, mYMinPos);

	mIntensity = percent;
	float xPos = (mIntensity*mXMaxPos - mIntensity*mXMinPos + mXMinPos);
	mSlider.setPosition(xPos, 0);

	mAttributeText.setString(mAttribute + " " + std::to_string((int)(mIntensity*100)));
	mAttributeText.move(-40, 40);

	mEventManager->attatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed, sf::Event::MouseMoved });
}

Slider::~Slider() {
	mEventManager->detatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed, sf::Event::MouseMoved });
}

void Slider::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mGauge, states);
	target.draw(mSlider, states);
	target.draw(mAttributeText, states);
}

void Slider::tick(const sf::Time & deltaTime) {

}

Slider::Strata Slider::getRenderLayer() const {
	return FIRST;
}

bool Slider::isAlive() const {
	return mIsAlive;
}

bool Slider::isActive() const {
	return mIsActive;
}

void Slider::setActive(const bool & active) {
	mIsActive = active;
}

void Slider::onEvent(const sf::Event & aEvent) {
	if (mIsActive) {
		sf::View GUIView(ViewUtility::getViewSize());
		sf::View mapView = mWindow->getView();
		mWindow->setView(GUIView);
		static sf::Vector2f mousePos(0, 0);
		sf::FloatRect rect(mSlider.getGlobalBounds());
		//Listen for mouse clicks
		if (aEvent.type == sf::Event::MouseButtonPressed&&
			aEvent.mouseButton.button == sf::Mouse::Left) {
			mousePos = mWindow->mapPixelToCoords(sf::Vector2i(aEvent.mouseButton.x, aEvent.mouseButton.y)) - getPosition();
			if (rect.contains(mousePos)) {
				mIsClicked = true;
			}
		}
		else if (mIsClicked && aEvent.type == sf::Event::EventType::MouseButtonReleased) {
			mIsClicked = false;
		}
		if (mIsClicked && aEvent.type == sf::Event::EventType::MouseMoved) {
			mousePos = VectorMath::projectVector(mWindow->mapPixelToCoords(sf::Vector2i(aEvent.mouseMove.x, aEvent.mouseMove.y)) - getPosition(), direction);
			float xPos = mousePos.x, yPos = mousePos.y;

			if (xPos >= mXMaxPos)
				xPos = mXMaxPos;
			else if (xPos <= mXMinPos)
				xPos = mXMinPos;

			if (yPos <= mYMaxPos)
				yPos = mYMaxPos;
			else if (yPos >= mYMinPos)
				yPos = mYMinPos;

			mSlider.setPosition(sf::Vector2f(xPos, yPos));

			mIntensity = (xPos - mXMinPos) / (mXMaxPos - mXMinPos);

			mAttributeText.setString(mAttribute + " " + std::to_string((int)(mIntensity * 100)));
			mOwner->onClick(mButtonFunc + std::to_string(mIntensity));
		}
		mWindow->setView(mapView);
	}
}

void Slider::setIntensity(float percent) {
	mIntensity = percent;
}

float Slider::getIntensity() const {
	return mIntensity;
}

void Slider::kill() {
	mIsAlive = false;
}
