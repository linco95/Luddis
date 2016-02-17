#ifndef _INCLUDED_DIALOGUE_
#define _INCLUDED_DIALOGUE_

#include <SFML/Graphics.hpp>
#include "InterfaceElement.h"
#include "Button.h"
#include "TextBox.h"
#include "CharacterPortrait.h"

class GUIManager;
class GameStateLevel;

class Dialogue : public InterfaceElement{
public:
	//The dialogue will want to have a pointer to a GameStateLevel
	//which is used to remove the inDialogue status.
	//If not in the GameStateLevel, then no pointer will
	//have to be passed on.
	Dialogue(const std::string& dialogueFile, sf::RenderWindow* window, GUIManager* guiManager, EventManager* eventManager, sf::Vector2f pos, GameStateLevel* gameStateLevel = nullptr);
	~Dialogue();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	RenderLayer getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	void setText(std::string newTextString);
	void onClick(std::string buttonFunc) override;

private:
	void addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos, int index);
	void initialize(std::string dialogueFile);
	void internalClear();
	void nextButton();
	void previousButton();
	void spiderButton1();
	void spiderButton2();
	void spiderButton3();
	void closeButton();
	
	sf::RenderWindow* mWindow;
	GameStateLevel* mGameStateLevel;
	GUIManager* mGUIManager;
	EventManager* mEventManager;
	int mLevel;
	bool mDrawContents;
	bool mIsAlive;
	bool mIsActive;
	CharacterPortrait* mCharacterDisplay;
	float mAnimationTimer;
	int mButtonCount;
	int mActivePage;
	sf::RectangleShape mBackground;
	typedef std::vector<TextBox> TextBoxVector;
	TextBox* mHeaders[10];
	TextBoxVector mDialogueTexts;
	typedef std::vector<Button*> ButtonVector;
	ButtonVector mButtons[10];
};

#endif // !_INCLUDED_DIALOGUE_
