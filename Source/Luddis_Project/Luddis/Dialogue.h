#ifndef _INCLUDED_DIALOGUE_
#define _INCLUDED_DIALOGUE_

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Button.h"
#include "TextBox.h"

class Dialogue : public Entity{
public:
	Dialogue(const std::string& dialogueFile, sf::RenderWindow* window, sf::Vector2f pos);
	~Dialogue();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;
	void updateText(const sf::Time& deltaTime);
	void setText(std::string newTextString);
private:
	void addButton(std::string buttonFile, std::string buttonText, sf::Vector2f pos);
	void initialize(std::string dialogueFile);

	sf::RenderWindow* mWindow;
	bool mIsAlive;
	bool mIsActive;
	int mButtonCount;
	int mActivePage;
	sf::Sprite mSprite;
	typedef std::vector<TextBox> TextBoxVector;
	TextBoxVector mDialogueTexts;
};

#endif // !_INCLUDED_DIALOGUE_
