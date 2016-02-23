#ifndef _INCLUDED_BUTTON_
#define _INCLUDED_BUTTON_

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>
#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
#include <SFML/Graphics/CircleShape.hpp>
#endif
#include "InterfaceElement.h"
#include "EventObserver.h"

class EventManager;

class Button : public InterfaceElement, public EventObserver{
public:
	enum ButtonType {
		CIRCLE,
		RECTANGLE
	};
	//This class goes by the assumption that the texture will
	//have three horizontal frames of equal size.
	//The first will be the default, the second the mouseover
	//and the third the click frame
	Button(std::string graphicFilename, std::string buttonText, std::string buttonFunc, sf::RenderWindow* window, EventManager* eventManager, sf::Vector2f pos, InterfaceElement* owner, ButtonType buttonType);
	virtual ~Button();

	void onEvent(const sf::Event &aEvent)override;

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	void kill();
	void setScale(sf::Vector2f& scale);
	void setScale(float x, float y);

private:
	void updateInput();

	InterfaceElement* mOwner;
	sf::RenderWindow* mWindow;
	EventManager* mEventManager;
	sf::Sprite mSprite;
	sf::IntRect mRects[3];
	sf::Text mButtonText;
	std::string mButtonFunc;
	ButtonType mButtonType;
#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	sf::CircleShape mDebugCircle;
#endif

	bool mIsAlive;
	bool mIsActive;
};

#endif // !_INCLUDED_BUTTON_
