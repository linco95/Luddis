#ifndef _INCLUDED_SLIDER_
#define _INCLUDED_SLIDER_

#include "InterfaceElement.h"
#include "EventObserver.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

class EventManager;


namespace sf {
	class RenderWindow;
}

class Slider : public InterfaceElement, public EventObserver{
public:
	Slider(std::string gaugeFile, std::string sliderFile, float percent, std::string attribute, std::string buttonFunc, sf::Vector2f pos, sf::RenderWindow* window, EventManager* eventManager, InterfaceElement* owner);
	~Slider();

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	void tick(const sf::Time & deltaTime) override;
	Strata getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool & active) override;
	void onEvent(const sf::Event & aEvent) override;

	void setIntensity(float percent);
	float getIntensity() const;
	void kill();

private:
	InterfaceElement* mOwner;
	EventManager* mEventManager;
	sf::RenderWindow* mWindow;
	sf::Sprite mGauge;
	sf::Sprite mSlider;
	sf::Text mAttributeText;
	std::string mButtonFunc;
	std::string mAttribute;

	float mXMinPos, mXMaxPos;
	float mYMinPos, mYMaxPos;
	float mIntensity;
	sf::Vector2f mLength;

	bool mIsClicked;
	bool mIsAlive;
	bool mIsActive;

};

#endif // !_INCLUDED_SLIDER_
