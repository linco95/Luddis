#ifndef _INCLUDED_BUTTON_
#define _INCLUDED_BUTTON_

#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Button: public Entity{
public:
	//This class goes by the assumption that the texture will
	//have three horizontal frames of equal size.
	//The first will be the default, the second the mouseover
	//and the third the click frame
	Button(std::string graphicFilename, sf::RenderWindow* window, sf::Vector2f pos);
	~Button();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;

private:
	void updateInput();
	void onClick();

	bool mIsAlive;
	bool mClicked;
	sf::RenderWindow* mWindow;
	sf::Sprite mSprite;
	sf::IntRect mRects[3];
};

#endif // !_INCLUDED_BUTTON_
