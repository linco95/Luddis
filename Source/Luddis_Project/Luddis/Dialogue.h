#ifndef _INCLUDED_DIALOGUE_
#define _INCLUDED_DIALOGUE_

#include <SFML/Graphics.hpp>
#include "InterfaceElement.h"
#include "Button.h"
#include "TextBox.h"
#include "CharacterPortrait.h"

class GUIManager;
class GameState;
class ResourceManager;
class SoundEngine;

class Dialogue : public InterfaceElement{
public:
	Dialogue(const std::string& dialogueFile, sf::RenderWindow* window, GUIManager* guiManager, EventManager* eventManager, sf::Vector2f pos, GameState* gameState, int initialPage = 0);
	~Dialogue();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	Strata getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	void onClick(std::string buttonFunc) override;

private:
	void addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos, int index);
	void initialize(std::string dialogueFile);
	void internalClear();
	void internalPageSwapAway(int value);
	void internalPageSwapInto(int value);
	void changePageButton(int value);
	void gotoPageButton(int value);
	void spiderButton1();
	void spiderButton2();
	void spiderButton3();
	void closeButton();

	static const int MAX_PAGES = 25;
	
	ResourceManager* mResourceManager;
	SoundEngine* mSoundEngine;
	sf::RenderWindow* mWindow;
	GameState* mGameState;
	GUIManager* mGUIManager;
	EventManager* mEventManager;
	int mLevel;
	bool mDrawContents;
	bool mIsAlive;
	bool mIsActive;
	CharacterPortrait* mCharacterDisplayLeft;
	CharacterPortrait* mCharacterDisplayRight;
	float mAnimationTimer;
	int mButtonCount;
	int mActivePage;
	int mCurrentVoiceDialogue;
	typedef std::vector<TextBox> TextBoxVector;
	TextBox* mHeaders[MAX_PAGES];
	TextBoxVector mDialogueTexts;
	typedef std::vector<Button*> ButtonVector;
	ButtonVector mButtons[MAX_PAGES];
	std::string mSoundFiles[MAX_PAGES];
	//The first array is which character, and the second on which page.
	int mEmotionFrame[2][MAX_PAGES];
	bool mHighlight[2][MAX_PAGES];
};

#endif // !_INCLUDED_DIALOGUE_
