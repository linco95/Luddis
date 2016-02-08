#ifndef _INCLUDED_BUTTON_
#define _INCLUDED_BUTTON_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity.h"

class Button : public Entity{
public:
	//This class goes by the assumption that the texture will
	//have three horizontal frames of equal size.
	//The first will be the default, the second the mouseover
	//and the third the click frame
	Button(std::string graphicFilename, sf::RenderWindow* window, sf::Vector2f pos, void(*action)(int, double) = 0);
	~Button();
	
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool& active) override;
	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;

	void setButtonPosition(sf::Vector2f pos);

private:
	void updateInput();
	void onClick();

	bool mClicked;
	sf::RenderWindow* mWindow;
	sf::Sprite mSprite;
	sf::IntRect mRects[3];

	bool mIsAlive;
	bool mIsActive;
};

#endif // !_INCLUDED_BUTTON_
