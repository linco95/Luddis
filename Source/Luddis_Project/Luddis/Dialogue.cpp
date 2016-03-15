#include "Dialogue.h"
#include "ResourceManager.h"
#include "GUIManager.h"
#include "GameStateLevel.h"
#include "SoundEngine.h"
#include "ViewUtility.h"
#include "Debug.h"
#include <rapidjson/document.h>
#include <cmath>
#include <array>
#include <typeinfo>

static const std::string FILENAME = "Resources/Configs/Levels/Level";

static const float ANIMATION_TIME = 0.9f;
static const float RECT_WIDTH = ViewUtility::VIEW_WIDTH;
static const float RECT_HEIGHT = 300;
static const int INDENT = 30;
static sf::IntRect DEFAULT_RECT(INDENT*7, INDENT, (int)RECT_WIDTH-INDENT*1, 0);

Dialogue::Dialogue(const std::string& dialogueFile, sf::RenderWindow* window, GUIManager* guiManager, EventManager* eventManager, sf::Vector2f pos, GameState* gameState, int initialPage) :
mButtonCount(0),
mAnimationTimer(ANIMATION_TIME),
mLevel(0),
mEmotionFrame(),
mIsAlive(true),
mIsActive(true),
mDrawContents(false),
mWindow(window),
mResourceManager(&ResourceManager::getInstance()),
mSoundEngine(&SoundEngine::getInstance()),
mGUIManager(guiManager),
mEventManager(eventManager),
mGameState(gameState),
mCharacterDisplayLeft(nullptr),
mCharacterDisplayRight(nullptr),
mActivePage(initialPage),
mDialogueTexts(){
	setPosition(pos);
	initialize(dialogueFile);
	static const char* parameter = "PauseMenu";
	SoundEngine::getInstance().setEventParameter("event:/Music/Levels/Lvl2", parameter, 1.0f);
}

Dialogue::~Dialogue(){
	internalClear();
	static const char* parameter = "PauseMenu";
	SoundEngine::getInstance().setEventParameter("event:/Music/Levels/Lvl2", parameter, 0.0f);
	mGameState->handleClicks("DeleteDialogue");
	mSoundEngine->stopSound(mCurrentVoiceDialogue);
}

void Dialogue::initialize(std::string dialogueFile){
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < MAX_PAGES; j++) {
			mEmotionFrame[i][j] = 0;
			mHighlight[i][j] = false;
		}
	}

	std::string configText = mResourceManager->loadJsonFile(dialogueFile);
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsObject());

	sf::Vector2f offset(0, -RECT_HEIGHT + INDENT*3);
	sf::Vector2f portraitPos(ViewUtility::VIEW_WIDTH*0.25f, offset.y+getPosition().y);

	//A character portrait is optional.
	if (configDoc.HasMember("Left_character_filename") &&
		configDoc.HasMember("Left_character_displayname")){

		std::string textureFilename = configDoc["Left_character_filename"].GetString();
		std::string characterName = configDoc["Left_character_displayname"].GetString();


		mCharacterDisplayLeft = new CharacterPortrait(textureFilename, characterName, portraitPos);
		mGUIManager->addInterfaceElement(mCharacterDisplayLeft);
	}
	//So is the second one
	if (configDoc.HasMember("Right_character_filename") &&
		configDoc.HasMember("Right_character_displayname")){

		std::string textureFilename = configDoc["Right_character_filename"].GetString();
		std::string characterName = configDoc["Right_character_displayname"].GetString();


		sf::Vector2f rightPortraitPos = portraitPos;
		rightPortraitPos.x = ViewUtility::VIEW_WIDTH*0.75f;
		mCharacterDisplayRight = new CharacterPortrait(textureFilename, characterName, rightPortraitPos, true);
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
			addButton(buttonImage, buttonText, buttonFunc, offset +  sf::Vector2f((float)INDENT*4, (float)INDENT + (float)i * 33.0f), (int)itr);
		}
		TextBox textBox(DEFAULT_RECT, pages[itr]["Text"].GetString(), 24, true);
		textBox.setPosition(offset);
		textBox.setColor(sf::Color::White);
		//textBox
		mDialogueTexts.push_back(textBox);
		if (pages[itr].HasMember("Header")){
			assert(pages[itr]["Header"].IsString());
			std::string text = pages[itr]["Header"].GetString();
			int fontSize = 32;

			mHeaders[itr] = new TextBox(DEFAULT_RECT, text, fontSize);
			mHeaders[itr]->setColor(sf::Color::White);
			mHeaders[itr]->setString(pages[itr]["Header"].GetString());

			sf::Vector2f headerOffset (0, (float)(mHeaders[itr]->getRows()*fontSize));
			mHeaders[itr]->setPosition(offset - headerOffset);
		}
		if (pages[itr].HasMember("Voice_file")) {
			assert(pages[itr]["Voice_file"].IsString());
			mSoundFiles[itr] = pages[itr]["Voice_file"].GetString();
		}
		if (pages[itr].HasMember("Emotion")) {
			assert(pages[itr]["Emotion"].IsObject());
			const rapidjson::Value& emotion = pages[itr]["Emotion"];
			assert(emotion.HasMember("Character"));
			assert(emotion.HasMember("Frame"));
			int character = emotion["Character"].GetInt();
			mEmotionFrame[character][itr] = emotion["Frame"].GetInt();
		}
		if (pages[itr].HasMember("Highlight")) {
			assert(pages[itr]["Highlight"].IsInt());
			int character = pages[itr]["Highlight"].GetInt();
			mHighlight[character][itr] = true;
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
	}
	else if(!mDrawContents){
		mDrawContents = true;
		if(mCharacterDisplayRight!= nullptr)
			mCharacterDisplayRight->setActive(true);
		if(mCharacterDisplayLeft!= nullptr)
			mCharacterDisplayLeft->setActive(true);
		internalPageSwapInto(mActivePage);
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

Dialogue::Strata Dialogue::getRenderLayer() const{
	return THIRD;
}

void Dialogue::addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos, int index){
	Button* button = new Button(buttonFile, buttonText, buttonFunc, mWindow, mEventManager, pos + getPosition(), this, Button::ButtonType::RECTANGLE);
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

void Dialogue::internalPageSwapAway(int value){
	/*if ((size_t)(mActivePage + value + 1)>mDialogueTexts.size() || (mActivePage + value)<0) {
		Debug::log("Trying to go outside of dialogue page index range!", Debug::FATAL);
		return;
	}*/
	for (ButtonVector::size_type i = 0; i < mButtons[mActivePage].size(); i++) {
		mButtons[mActivePage][i]->setActive(false);
	}
	if (mSoundFiles[mActivePage].size() != 0)
		//TODO: Make this use studio instead of low level api
		mSoundEngine->stopSound(mCurrentVoiceDialogue);
}

void Dialogue::internalPageSwapInto(int value){
	for (ButtonVector::size_type i = 0; i < mButtons[mActivePage].size(); i++) {
		mButtons[mActivePage][i]->setActive(true);
	}
	if (mSoundFiles[mActivePage].size() != 0) {
		//TODO: Make this use studio instead of low level api
		mCurrentVoiceDialogue = mSoundEngine->playSound(mSoundFiles[mActivePage].c_str());
	}
	if (mCharacterDisplayLeft != nullptr) {
		mCharacterDisplayLeft->expressEmotion(mEmotionFrame[0][mActivePage]);
		mCharacterDisplayLeft->highlight(mHighlight[0][mActivePage]);
	}
	if (mCharacterDisplayRight != nullptr) {
		mCharacterDisplayRight->expressEmotion(mEmotionFrame[1][mActivePage]);
		mCharacterDisplayRight->highlight(mHighlight[1][mActivePage]);
	}
}

void Dialogue::changePageButton(int value){
	internalPageSwapAway(mActivePage);
	mActivePage += value;
	internalPageSwapInto(mActivePage);
}

void Dialogue::gotoPageButton(int value){
	internalPageSwapAway(mActivePage);
	mActivePage = value-1;
	internalPageSwapInto(mActivePage);
}

void Dialogue::closeButton(){
	mIsAlive = false;
}

void Dialogue::spiderButton1(){
	changePageButton(1);
	mGameState->handleClicks("Spider1");
}

void Dialogue::spiderButton2(){
	changePageButton(2);
	mGameState->handleClicks("Spider2");
}

void Dialogue::spiderButton3(){
	changePageButton(3);
	mGameState->handleClicks("Spider3");
}

//Call the function corresonding to the string passed back.
void Dialogue::onClick(std::string buttonFunc){
	std::string substringPage = buttonFunc.substr(0, 4);
	std::string substringGotoPage = buttonFunc.substr(0, 8);
	if (substringPage == "Page"){
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
	else if (substringGotoPage == "GotoPage") {
		std::string string = buttonFunc.substr(8, buttonFunc.size());
		int value = std::stoi(string);
		gotoPageButton(value);
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
