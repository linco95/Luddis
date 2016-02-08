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

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	void updateText(const sf::Time& deltaTime);
	void addButton(std::string buttonFile, sf::Vector2f pos);
private:
	sf::RenderWindow* mWindow;
	bool mIsAlive;
	bool mIsActive;
	int mButtonCount;
	sf::Sprite mSprite;
	TextBox mDialogueText;
	Button* mButtons[4];
};

#endif // !_INCLUDED_DIALOGUE_
