#ifndef _INCLUDED_BUTTON_
#define _INCLUDED_BUTTON_

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#endif
#include "InterfaceElement.h"
#include "EventObserver.h"

class Button : public InterfaceElement, public EventObserver{
public:
	//This class goes by the assumption that the texture will
	//have three horizontal frames of equal size.
	//The first will be the default, the second the mouseover
	//and the third the click frame
	Button(std::string graphicFilename, std::string buttonText, std::string buttonFunc, sf::RenderWindow* window, sf::Vector2f pos, InterfaceElement* owner);
	virtual ~Button();

	void update(const sf::Event &aEvent)override;

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	void kill();

private:
	void updateInput();

	InterfaceElement* mOwner;
	sf::RenderWindow* mWindow;
	sf::Sprite mSprite;
	sf::IntRect mRects[3];
	sf::Text mButtonText;
	std::string mButtonFunc;

#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	sf::RectangleShape mDebugRect;
	sf::CircleShape mDebugCircle;
#endif

	bool mIsAlive;
	bool mIsActive;
};

#endif // !_INCLUDED_BUTTON_
