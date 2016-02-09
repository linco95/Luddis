#include "Dialogue.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "Utils.h"
#include <rapidjson/document.h>
#include <array>

static const std::string DIALOGUE_TEXTURE = "Resources/Images/Parchment.png";
static const std::array<std::string, 3> CONFIGMEMBERS = { "Character_filename", "Character_displayname", "Pages" };
static sf::IntRect DEFAULT_RECT(30, 30, 400, 0);

Dialogue::Dialogue(const std::string& dialogueFile, sf::RenderWindow* window, sf::Vector2f pos):
mButtonCount(0),
mIsAlive(true),
mIsActive(true),
mWindow(window),
mActivePage(0),
mSprite(ResourceManager::getInstance().getTexture(DIALOGUE_TEXTURE)),
mDialogueTexts(){
	initialize(dialogueFile);
	setPosition(pos);
}

Dialogue::~Dialogue(){
	
}

void Dialogue::initialize(std::string dialogueFile){
	std::string configText = LuddisUtilFuncs::loadJsonFile(dialogueFile);
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsObject());

	for (std::size_t i = 0; i < CONFIGMEMBERS.size(); i++){
		assert(configDoc.HasMember(CONFIGMEMBERS[i].c_str()));
	}

	const rapidjson::Value& pages = configDoc["Pages"];
	assert(pages.IsArray());
	for (rapidjson::SizeType itr = 0; itr < pages.Size(); itr++){
		//Considering making a header optional

		//TODO: Implement an image to display which character is talking in the dialogue
		assert(pages[itr].HasMember("Header") && pages[itr]["Header"].IsString());
		assert(pages[itr].HasMember("Text") && pages[itr]["Text"].IsString());
		assert(pages[itr].HasMember("Buttons") && pages[itr]["Buttons"].IsArray());
		const rapidjson::Value& buttons = pages[itr]["Buttons"];
		for (rapidjson::SizeType i = 0; i < buttons.Size(); i++){
			const rapidjson::Value& buttonInfo = buttons[i];
			assert(buttonInfo.IsObject());
			assert(buttonInfo.HasMember("Button_text") && buttonInfo["Button_text"].IsString());
			assert(buttonInfo.HasMember("Button_image") && buttonInfo["Button_image"].IsString());

			addButton(buttonInfo["Button_image"].GetString(), buttons[i]["Button_text"].GetString(), sf::Vector2f(120 + (float)i * 75, 400));

		}
		TextBox textBox(DEFAULT_RECT, pages[itr]["Text"].GetString(), 24);
		mDialogueTexts.push_back(textBox);
	}
}

void Dialogue::tick(const sf::Time& deltaTime){
	updateText(deltaTime);
}

void Dialogue::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mDialogueTexts[mActivePage], states);
	/*for (int i = 0; i < mButtonCount; i++){
		target.draw(*mButtons[i], states);
	}*/
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
	return GUI_BACKGROUND;
}

void Dialogue::addButton(std::string buttonFile, std::string buttonText, sf::Vector2f pos){
	Button* button = new Button(buttonFile, buttonText, mWindow, pos);
	EntityManager::getInstance().addEntity(button);
}