#include "Dialogue.h"
#include "ResourceManager.h"
#include "GUIManager.h"
#include "Utils.h"
#include <rapidjson/document.h>
#include <array>

static const std::string DIALOGUE_TEXTURE = "Resources/Images/Parchment.png";
static const std::array<std::string, 4> CONFIGMEMBERS = { "Character_filename", "Character_displayname", "Header", "Pages" };
static sf::IntRect DEFAULT_RECT(30, 30, 400, 0);

Dialogue::Dialogue(const std::string& dialogueFile, sf::RenderWindow* window, GUIManager* guiManager, EventManager* eventManager, sf::Vector2f pos) :
mButtonCount(0),
mIsAlive(true),
mIsActive(true),
mWindow(window),
mGUIManager(guiManager),
mEventManager(eventManager),
mActivePage(0),
mSprite(ResourceManager::getInstance().getTexture(DIALOGUE_TEXTURE)),
mDialogueTexts(),
mHeader(DEFAULT_RECT, "", 28){
	setPosition(pos);
	initialize(dialogueFile);
}

Dialogue::~Dialogue(){
	internalClear();
}

void Dialogue::initialize(std::string dialogueFile){
	std::string configText = LuddisUtilFuncs::loadJsonFile(dialogueFile);
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsObject());

	for (std::size_t i = 0; i < CONFIGMEMBERS.size(); i++){
		assert(configDoc.HasMember(CONFIGMEMBERS[i].c_str()));
	}

	std::string textureFilename = configDoc["Character_filename"].GetString();
	std::string characterName = configDoc["Character_displayname"].GetString();
	sf::Vector2f pos = getPosition();
	
	mCharacterDisplay = new CharacterPortrait(textureFilename, characterName, pos);
	mGUIManager->addInterfaceElement(mCharacterDisplay);

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
			addButton(buttonImage, buttonText, buttonFunc, sf::Vector2f(80 + (float)i * 100, 300), (int)itr);

		}
		TextBox textBox(DEFAULT_RECT, pages[itr]["Text"].GetString(), 24);
		mDialogueTexts.push_back(textBox);
		if (itr == 0){
			mDialogueTexts.back().move(0, 40);
		}
	}
	for (ButtonVector::size_type i = 0; i < mButtons[0].size(); i++){
		mButtons[0].at(i)->setActive(true);
	}
}

void Dialogue::tick(const sf::Time& deltaTime){
	
}

void Dialogue::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mDialogueTexts[mActivePage], states);
	if (mActivePage == 0){
		target.draw(mHeader, states);
	}
}

void Dialogue::updateText(const sf::Time& deltaTime){
	
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