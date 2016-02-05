#ifndef _INCLUDED_DIALOGUE_
#define _INCLUDED_DIALOGUE_

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Button.h"
#include "TextBox.h"

class Dialogue : public Entity{
public:
	Dialogue(std::string text, sf::RenderWindow* window, sf::Vector2f pos);
	~Dialogue();
	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;
	void updateText(const sf::Time& deltaTime);
	void addButton(std::string buttonFile, sf::Vector2f pos);
private:
	sf::RenderWindow* mWindow;
	bool mIsAlive;
	int mButtonCount;
	sf::Sprite mSprite;
	TextBox mDialogueText;
	Button* mButtons[4];
};

#endif // !_INCLUDED_DIALOGUE_
