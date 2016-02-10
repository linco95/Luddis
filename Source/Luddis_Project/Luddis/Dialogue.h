#ifndef _INCLUDED_DIALOGUE_
#define _INCLUDED_DIALOGUE_

#include <SFML/Graphics.hpp>
#include "InterfaceElement.h"
#include "Button.h"
#include "TextBox.h"
#include "CharacterPortrait.h"

class Dialogue : public InterfaceElement{
public:
	Dialogue(const std::string& dialogueFile, sf::RenderWindow* window, sf::Vector2f pos);
	~Dialogue();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	void updateText(const sf::Time& deltaTime);
	void setText(std::string newTextString);
	void onClick(std::string buttonType) override;
private:
	void addButton(std::string buttonFile, std::string buttonText, sf::Vector2f pos, int index);
	void initialize(std::string dialogueFile);
	void internalClear();
	void nextButton();
	void previousButton();
	void okButton();
	
	sf::RenderWindow* mWindow;
	bool mIsAlive;
	bool mIsActive;
	CharacterPortrait* mCharacterDisplay;
	int mButtonCount;
	int mActivePage;
	sf::Sprite mSprite;
	TextBox mHeader;
	typedef std::vector<TextBox> TextBoxVector;
	TextBoxVector mDialogueTexts;
	typedef std::vector<Button*> ButtonVector;
	ButtonVector mButtons[10];
};

#endif // !_INCLUDED_DIALOGUE_
