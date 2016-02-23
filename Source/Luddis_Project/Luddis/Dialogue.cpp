#include "Dialogue.h"
#include "ResourceManager.h"
#include "GUIManager.h"
#include "GameStateLevel.h"
#include "SoundEngine.h"
#include "ViewUtility.h"
#include <rapidjson/document.h>
#include <cmath>
#include <array>
#include <typeinfo>

static const std::string BACKGROUND_TEXTURE = "Resources/Images/GUI/DialogueFrame.png";
static const std::string FILENAME = "Resources/Configs/Levels/Level";

static const float ANIMATION_TIME = 1.5f;
static const float RECT_WIDTH = ViewUtility::VIEW_WIDTH;
static const float RECT_HEIGHT = 600;
static const int INDENT = 30;
static sf::Vector2f DialogueBoxMaxSize(RECT_WIDTH, RECT_HEIGHT);
static sf::IntRect DEFAULT_RECT(INDENT*6, INDENT, (int)RECT_WIDTH-INDENT*2, 0);

Dialogue::Dialogue(const std::string& dialogueFile, sf::RenderWindow* window, GUIManager* guiManager, EventManager* eventManager, sf::Vector2f pos, GameStateLevel* gameStateLevel) :
mButtonCount(0),
mAnimationTimer(ANIMATION_TIME),
mLevel(0),
mIsAlive(true),
mIsActive(true),
mDrawContents(false),
mWindow(window),
mResourceManager(&ResourceManager::getInstance()),
mSoundEngine(&SoundEngine::getInstance()),
mGUIManager(guiManager),
mEventManager(eventManager),
mGameStateLevel(gameStateLevel),
mCharacterDisplayLeft(nullptr),
mCharacterDisplayRight(nullptr),
mActivePage(0),
mBackground(),
mDialogueTexts(){
	setPosition(pos);
	sf::Vector2f offset(0, -RECT_HEIGHT);
	mBackground.setPosition(offset);
	mBackground.setTexture(&mResourceManager->getTexture(BACKGROUND_TEXTURE));
	mBackground.setFillColor(sf::Color(255, 255, 255));
	//mBackground.setOutlineThickness((float)INDENT / 2);
	initialize(dialogueFile);
}

Dialogue::~Dialogue(){
	internalClear();
	if (mGameStateLevel != nullptr){
		mGameStateLevel->setInDialogue(false);
	}
	mSoundEngine->stopSound(mCurrentVoiceDialogue);
}

void Dialogue::initialize(std::string dialogueFile){
	std::string configText = mResourceManager->loadJsonFile(dialogueFile);
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsObject());

	sf::Vector2f offset(0, -RECT_HEIGHT);
	sf::Vector2f pos = getPosition() + offset;

	//A character portrait is optional.
	if (configDoc.HasMember("Left_character_filename") &&
		configDoc.HasMember("Left_character_displayname")){

		std::string textureFilename = configDoc["Left_character_filename"].GetString();
		std::string characterName = configDoc["Left_character_displayname"].GetString();


		mCharacterDisplayLeft = new CharacterPortrait(textureFilename, characterName, pos);
		mGUIManager->addInterfaceElement(mCharacterDisplayLeft);
	}
	//So is the second one
	if (configDoc.HasMember("Right_character_filename") &&
		configDoc.HasMember("Right_character_displayname")){

		std::string textureFilename = configDoc["Right_character_filename"].GetString();
		std::string characterName = configDoc["Right_character_displayname"].GetString();


		sf::Vector2f rightPos = pos;
		rightPos.x += 600;
		mCharacterDisplayRight = new CharacterPortrait(textureFilename, characterName, rightPos);
		mGUIManager->addInterfaceElement(mCharacterDisplayRight);
	}

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
			addButton(buttonImage, buttonText, buttonFunc, mBackground.getPosition() +  sf::Vector2f((float)INDENT*2, (float)INDENT*2 + (float)i * 60.0f), (int)itr);

		}
		TextBox textBox(DEFAULT_RECT, pages[itr]["Text"].GetString(), 24, true);
		textBox.setPosition(offset);
		mDialogueTexts.push_back(textBox);
		if (pages[itr].HasMember("Header")){
			assert(pages[itr]["Header"].IsString());
			std::string text = pages[itr]["Header"].GetString();
			int fontSize = 32;

			mHeaders[itr] = new TextBox(DEFAULT_RECT, text, fontSize);
			mHeaders[itr]->setString(pages[itr]["Header"].GetString());
			mHeaders[itr]->setPosition(offset);

			int headerOffset = mHeaders[itr]->getRows()*fontSize;
			mDialogueTexts.back().move(0.0f, (float)headerOffset);
		}
		if (pages[itr].HasMember("Voice_file")) {
			assert(pages[itr]["Voice_file"].IsString());
			mSoundFiles[itr] = pages[itr]["Voice_file"].GetString();
			mResourceManager->loadSoundBuffer(mSoundFiles[itr]);
		}
	}
	if (configDoc.HasMember("Level")){
		assert(configDoc["Level"].IsInt());
		mLevel = configDoc["Level"].GetInt();
	}
}

void Dialogue::tick(const sf::Time& deltaTime){
	if (mAnimationTimer > 0){
		mAnimationTimer -= deltaTime.asSeconds();
		mAnimationTimer = std::max(mAnimationTimer, 0.0f);
		float height = RECT_HEIGHT*((ANIMATION_TIME - mAnimationTimer) / ANIMATION_TIME);
		sf::Vector2f size(RECT_WIDTH, height);
		sf::Vector2f position(0, -height);

		mBackground.setSize(size);
		mBackground.setPosition(position);
	}
	else if(!mDrawContents){
		mDrawContents = true;
		if(mCharacterDisplayRight!= nullptr)
			mCharacterDisplayRight->setActive(true);
		if(mCharacterDisplayLeft!= nullptr)
			mCharacterDisplayLeft->setActive(true);
		if(mSoundFiles[mActivePage].size() != 0)
			mCurrentVoiceDialogue = mSoundEngine->playSound(mSoundFiles[mActivePage]);
		for (ButtonVector::size_type i = 0; i < mButtons[mActivePage].size(); i++){
			mButtons[mActivePage].at(i)->setActive(true);
		}
	}
	if (mDrawContents){
		mDialogueTexts[mActivePage].animate(deltaTime);
		if (mHeaders[mActivePage] != nullptr){
			mHeaders[mActivePage]->animate(deltaTime);
		}
	}
}

void Dialogue::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mBackground, states);
	if (mDrawContents){
		target.draw(mDialogueTexts[mActivePage], states);
		if (mHeaders[mActivePage] != nullptr){
			target.draw(*mHeaders[mActivePage], states);
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
	if (mCharacterDisplayRight != nullptr) {
		mCharacterDisplayRight->kill();
		mCharacterDisplayRight = nullptr;
	}
	if (mCharacterDisplayLeft != nullptr) {
		mCharacterDisplayLeft->kill();
		mCharacterDisplayLeft = nullptr;
	}
	for (TextBoxVector::size_type i = 0; i < mDialogueTexts.size(); i++){
		for (ButtonVector::size_type j = 0; j < mButtons[i].size(); j++){
			mButtons[i][j]->kill();
		}
		if (mHeaders[i] != nullptr){
			delete mHeaders[i];
		}
	}
}

void Dialogue::changePageButton(int value){
	for (ButtonVector::size_type i = 0; i < mButtons[mActivePage].size(); i++){
		mButtons[mActivePage][i]->setActive(false);
	}
	if (mSoundFiles[mActivePage].size() != 0)
		mSoundEngine->stopSound(mCurrentVoiceDialogue);
	mActivePage += value;
	for (ButtonVector::size_type i = 0; i < mButtons[mActivePage].size(); i++){
		mButtons[mActivePage][i]->setActive(true);
	}
	if (mSoundFiles[mActivePage].size() != 0)
		mCurrentVoiceDialogue = mSoundEngine->playSound(mSoundFiles[mActivePage]);
}

void Dialogue::closeButton(){
	mIsAlive = false;
}

void Dialogue::spiderButton1(){
	//Read a json file and a png file to create extra entities
	std::string jsonFilename = FILENAME + std::to_string( mLevel) + "SpiderEasy.json";
	std::string mapFilename = FILENAME + std::to_string(mLevel) + "SpiderEasy.png";
	changePageButton(1);
	mGameStateLevel->setupMission(mapFilename, jsonFilename);
}

void Dialogue::spiderButton2(){
	//Read a json file and a png file to create extra entities
	std::string jsonFilename = FILENAME + std::to_string(mLevel) + "SpiderMedium.json";
	std::string mapFilename = FILENAME + std::to_string(mLevel) + "SpiderMedium.png";
	changePageButton(2);
	mGameStateLevel->setupMission(mapFilename, jsonFilename);
}

void Dialogue::spiderButton3(){
	//Read a json file and a png file to create extra entities
	std::string jsonFilename = FILENAME + std::to_string(mLevel) + "SpiderHard.json";
	std::string mapFilename = FILENAME + std::to_string(mLevel) + "SpiderHard.png";
	changePageButton(3);
	mGameStateLevel->setupMission(mapFilename, jsonFilename);
}

//Call the function corresonding to the string passed back.
void Dialogue::onClick(std::string buttonFunc){
	std::string substring = buttonFunc.substr(0, 4);
	if (substring == "Page"){
		int value = 0;
		std::string string = buttonFunc.substr(5, buttonFunc.size());
		if (buttonFunc.at(4) == '+'){
			value = std::stoi(string);
		}
		else if (buttonFunc.at(4) == '-'){
			value = -std::stoi(string);
		}
		changePageButton(value);
	}
	else if (buttonFunc == "Close"){
		closeButton();
	}
	else if (buttonFunc == "Spider1"){
		spiderButton1();

	}
	else if (buttonFunc == "Spider2"){
		spiderButton2();
	}
	else if (buttonFunc == "Spider3"){
		spiderButton3();
	}
}
