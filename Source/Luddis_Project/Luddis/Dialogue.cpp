#include "Dialogue.h"
#include "ResourceManager.h"
#include "GUIManager.h"
#include <rapidjson/document.h>
#include <cmath>
#include <array>

static const std::array<std::string, 4> CONFIGMEMBERS = { "Character_filename", "Character_displayname", "Header", "Pages" };
static const std::string BACKGROUND_TEXTURE = "Resources/Images/GUI/DialogueFrame.png";

static const float ANIMATION_TIME = 1.5f;
static const float RECT_WIDTH = 1000;
static const float RECT_HEIGHT = 600;
static const int INDENT = 30;
static sf::Vector2f DialogueBoxMaxSize(RECT_WIDTH, RECT_HEIGHT);
static sf::IntRect DEFAULT_RECT(INDENT, INDENT, (int)RECT_WIDTH-INDENT*2, 0);

Dialogue::Dialogue(const std::string& dialogueFile, sf::RenderWindow* window, GUIManager* guiManager, EventManager* eventManager, sf::Vector2f pos) :
mButtonCount(0),
mAnimationTimer(ANIMATION_TIME),
mIsAlive(true),
mIsActive(true),
mDrawContents(false),
mWindow(window),
mGUIManager(guiManager),
mEventManager(eventManager),
mActivePage(0),
mBackground(),
mDialogueTexts(),
mHeader(DEFAULT_RECT, "", 28){
	setPosition(pos);
	sf::Vector2f offset(0, -RECT_HEIGHT);
	mBackground.setPosition(offset);
	mBackground.setTexture(&ResourceManager::getInstance().getTexture(BACKGROUND_TEXTURE));
	mHeader.setPosition(offset);
	mBackground.setFillColor(sf::Color(255, 255, 255));
	mBackground.setOutlineThickness((float)INDENT / 2);
	initialize(dialogueFile);
}

Dialogue::~Dialogue(){
	internalClear();
}

void Dialogue::initialize(std::string dialogueFile){
	std::string configText = ResourceManager::getInstance().loadJsonFile(dialogueFile);
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsObject());

	for (std::size_t i = 0; i < CONFIGMEMBERS.size(); i++){
		assert(configDoc.HasMember(CONFIGMEMBERS[i].c_str()));
	}

	std::string textureFilename = configDoc["Character_filename"].GetString();
	std::string characterName = configDoc["Character_displayname"].GetString();
	
	sf::Vector2f offset(0, -RECT_HEIGHT);
	sf::Vector2f pos = getPosition() + offset;
	
	mCharacterDisplay = new CharacterPortrait(textureFilename, characterName, pos);
	mGUIManager->addInterfaceElement(mCharacterDisplay);
	mCharacterDisplay->setActive(false);

	mHeader.setString(configDoc["Header"].GetString());
	const rapidjson::Value& pages = configDoc["Pages"];
	assert(pages.IsArray());
	for (rapidjson::SizeType itr = 0; itr < pages.Size(); itr++){

		assert(pages[itr].HasMember("Text") && pages[itr]["Text"].IsString());
		assert(pages[itr].HasMember("Buttons") && pages[itr]["Buttons"].IsArray());
		const rapidjson::Value& buttons = pages[itr]["Buttons"];
		for (rapidjson::SizeType i = 0; i < buttons.Size(); i++){
			const rapidjson::Value& buttonInfo = buttons[i];
			assert(buttonInfo.IsObject());
			assert(buttonInfo.HasMember("Button_text") && buttonInfo["Button_text"].IsString());
			std::string buttonText = buttonInfo["Button_text"].GetString();
			assert(buttonInfo.HasMember("Button_image") && buttonInfo["Button_image"].IsString());
			std::string buttonImage = buttonInfo["Button_image"].GetString();
			assert(buttonInfo.HasMember("Button_func") && buttonInfo["Button_func"].IsString());
			std::string buttonFunc = buttonInfo["Button_func"].GetString();
			addButton(buttonImage, buttonText, buttonFunc,mBackground.getPosition() +  sf::Vector2f(80 + (float)i * 100, RECT_HEIGHT-(float)INDENT*2.0f), (int)itr);

		}
		TextBox textBox(DEFAULT_RECT, pages[itr]["Text"].GetString(), 24);
		textBox.setPosition(offset);
		mDialogueTexts.push_back(textBox);
		if (itr == 0){
			mDialogueTexts.back().move(0, 40);
		}
	}
}

void Dialogue::tick(const sf::Time& deltaTime){
	if (mAnimationTimer >= 0){
		mAnimationTimer -= deltaTime.asSeconds();
		std::max(mAnimationTimer, 0.0f);
		float height = RECT_HEIGHT*((ANIMATION_TIME - mAnimationTimer) / ANIMATION_TIME);
		sf::Vector2f size(RECT_WIDTH, height);
		sf::Vector2f position(0, -height);

		mBackground.setSize(size);
		mBackground.setPosition(position);
	}
	else if(!mDrawContents){
		mDrawContents = true;
		mCharacterDisplay->setActive(true);
		for (ButtonVector::size_type i = 0; i < mButtons[0].size(); i++){
			mButtons[0].at(i)->setActive(true);
		}
	}
}

void Dialogue::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mBackground, states);
	if (mDrawContents){
		target.draw(mDialogueTexts[mActivePage], states);
		if (mActivePage == 0){
			target.draw(mHeader, states);
		}
	}
}

bool Dialogue::isAlive() const{
	return mIsAlive;
}

bool Dialogue::isActive() const{
	return mIsActive;
}

void Dialogue::setActive(const bool& active){
	mIsActive = active;
}

Dialogue::RenderLayer Dialogue::getRenderLayer() const{
	return BACKGROUND;
}

void Dialogue::addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos, int index){
	Button* button = new Button(buttonFile, buttonText, buttonFunc, mWindow, mEventManager, pos + getPosition(), this);
	mButtons[index].push_back(button);
	mGUIManager->addInterfaceElement(button);
}

void Dialogue::internalClear(){
	mCharacterDisplay->kill();
	for (int i = 0; i < mActivePage+1;i++){
		for (ButtonVector::size_type j = 0; j < mButtons[i].size(); j++){
			mButtons[i][j]->kill();
		}
	}
}

void Dialogue::nextButton(){
	for (ButtonVector::size_type i = 0; i < mButtons[mActivePage].size(); i++){
		mButtons[mActivePage][i]->setActive(false);
	}
	mActivePage += 1;
	for (ButtonVector::size_type i = 0; i < mButtons[mActivePage].size(); i++){
		mButtons[mActivePage][i]->setActive(true);
	}
}

void Dialogue::previousButton(){
	for (ButtonVector::size_type i = 0; i < mButtons[mActivePage].size(); i++){
		mButtons[mActivePage][i]->setActive(false);
	}
	mActivePage -= 1;
	for (ButtonVector::size_type i = 0; i < mButtons[mActivePage].size(); i++){
		mButtons[mActivePage][i]->setActive(true);
	}
}

void Dialogue::closeButton(){
	mIsAlive = false;
}

void Dialogue::onClick(std::string buttonFunc){
	if (buttonFunc == "Next"){
		nextButton();
	}
	else if (buttonFunc == "Previous"){
		previousButton();
	}
	else if (buttonFunc == "Close"){
		closeButton();
	}
}