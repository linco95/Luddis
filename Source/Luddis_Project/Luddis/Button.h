#ifndef _INCLUDED_BUTTON_
#define _INCLUDED_BUTTON_

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "InterfaceElement.h"

class Button : public InterfaceElement{
public:
	//This class goes by the assumption that the texture will
	//have three horizontal frames of equal size.
	//The first will be the default, the second the mouseover
	//and the third the click frame
	Button(std::string graphicFilename, std::string buttonText, sf::RenderWindow* window, sf::Vector2f pos, void(*action)() = 0);
	virtual ~Button();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	void onClick(void(*action)());
	

private:
	void updateInput();

	sf::RenderWindow* mWindow;
	sf::Sprite mSprite;
	sf::IntRect mRects[3];
	sf::Text mButtonText;

	bool mClicked;
	bool mIsAlive;
	bool mIsActive;
};

#endif // !_INCLUDED_BUTTON_
