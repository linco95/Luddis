#include "HUD.h"
#include "GUIManager.h"
#include "EventManager.h"
#include "Button.h"
#include "ScoreCounter.h"
#include "ScoreGauge.h"
#include "SoundEngine.h"
#include "Inventory.h"
#include "GameState.h"
#include "ViewUtility.h"
#include "ResourceManager.h"
#include <SFML/Graphics/RenderWindow.hpp>

static const char* TEXTURE_LUDDGAUGE_FRAME = "Resources/Images/GUI/LuddGaugeFrame.png";
static const char* TEXTURE_LUDDGAUGE_BAR = "Resources/Images/GUI/LuddGaugeBar.png";

static const char* TEXTURE_CHIPSCOUNTER = "Resources/Images/GUI/HUD_Chips_Icon.png";
static const char* TEXTURE_LUDDCOUNTER = "Resources/Images/GUI/HUD_Ludd_Icon.png";
static const char* TEXTURE_SOUNDBUTTON = "Resources/Images/GUI/HUD_sound_button.png";
static const char* TEXTURE_MUTEBUTTON = "Resources/Images/GUI/HUD_mute.png";

HUD::HUD(sf::RenderWindow* window, GUIManager* guiManager, EventManager* eventManager, GameState* gameState) :
	mIsActive(true),
	mIsAlive(true),
	mMute(false),
	mWindow(window),
	mGUIManager(guiManager),
	mEventManager(eventManager),
	mGameState(gameState) {

}

HUD::~HUD() {
	if (mLuddGauge != nullptr)
		mLuddGauge->kill();
}

void HUD::initialize(HUDType hudType) {
	sf::Vector2f position;
	switch (hudType)
	{
	case HUD::LEVEL:
		position = sf::Vector2f(ViewUtility::VIEW_WIDTH*0.45f, ViewUtility::VIEW_HEIGHT - 60);
		mLuddGauge = new ScoreGauge(mWindow, TEXTURE_LUDDGAUGE_FRAME, TEXTURE_LUDDGAUGE_BAR, sf::Vector2f(ViewUtility::VIEW_WIDTH * 0.45f, ViewUtility::VIEW_HEIGHT - 60));
		mGUIManager->addInterfaceElement(mLuddGauge);

		position.x = ViewUtility::VIEW_WIDTH*0.7f;
		mScoreCounter[0] = new ScoreCounter(mWindow, TEXTURE_CHIPSCOUNTER, position, ScoreCounter::ScoreType::CHIPS);
		mScoreCounter[0]->setScore(Inventory::getInstance().getChips());
		mGUIManager->addInterfaceElement(mScoreCounter[0]);

		position.x = ViewUtility::VIEW_WIDTH * 0.3f;
		mScoreCounter[1] = new ScoreCounter(mWindow, TEXTURE_LUDDCOUNTER, position, ScoreCounter::ScoreType::DUST);
		mScoreCounter[1]->setScore(Inventory::getInstance().getDust());
		mGUIManager->addInterfaceElement(mScoreCounter[1]);

		position.x = ViewUtility::VIEW_WIDTH * 0.85f;
		mSoundButton = new Button(TEXTURE_SOUNDBUTTON, "", "Mute", mWindow, mEventManager, position, this, Button::RECTANGLE);
		mSoundButton->setActive(true);
		mGUIManager->addInterfaceElement(mSoundButton);

		mMuteGraphic.setTexture(ResourceManager::getInstance().getTexture(TEXTURE_MUTEBUTTON));
		mMuteGraphic.setScale(2.0f, 2.0f);
		mMuteGraphic.setOrigin(mMuteGraphic.getLocalBounds().width / 2, mMuteGraphic.getLocalBounds().height / 2);
		mMuteGraphic.setPosition(position);
		break;

	case HUD::MAP:
		position = sf::Vector2f(ViewUtility::VIEW_WIDTH*0.7f, ViewUtility::VIEW_HEIGHT - 60);
		mScoreCounter[0] = new ScoreCounter(mWindow, TEXTURE_CHIPSCOUNTER, position, ScoreCounter::ScoreType::CHIPS);
		mScoreCounter[0]->setScore(Inventory::getInstance().getChips());
		mGUIManager->addInterfaceElement(mScoreCounter[0]);

		position.x = ViewUtility::VIEW_WIDTH * 0.3f;
		mScoreCounter[1] = new ScoreCounter(mWindow, TEXTURE_LUDDCOUNTER, position, ScoreCounter::ScoreType::DUST);
		mScoreCounter[1]->setScore(Inventory::getInstance().getDust());
		mGUIManager->addInterfaceElement(mScoreCounter[1]);

		position.x = ViewUtility::VIEW_WIDTH * 0.85f;
		mSoundButton = new Button(TEXTURE_SOUNDBUTTON, "", "Mute", mWindow, mEventManager, position, this, Button::RECTANGLE);
		mSoundButton->setActive(true);
		mGUIManager->addInterfaceElement(mSoundButton);

		mMuteGraphic.setTexture(ResourceManager::getInstance().getTexture(TEXTURE_MUTEBUTTON));
		mMuteGraphic.setScale(2.0f, 2.0f);
		mMuteGraphic.setOrigin(mMuteGraphic.getLocalBounds().width / 2, mMuteGraphic.getLocalBounds().height / 2);
		mMuteGraphic.setPosition(position);
		break;

	default:
		//Do nothing
		break;
	}
}

void HUD::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if (mMute) {
		states.transform *= getTransform();
		target.draw(mMuteGraphic, states);
	}
}

void HUD::tick(const sf::Time & deltaTime) {
	if (mLuddGauge != nullptr) {
		int maxDust = Inventory::getInstance().getMaxDust();
		int currentDust = Inventory::getInstance().getDust();
		float fillPercent = (float)currentDust / (float)maxDust;
		mLuddGauge->updateGauge(fillPercent);
	}
}

HUD::Strata HUD::getRenderLayer() const {
	return InterfaceElement::Strata::FIRST;
}

bool HUD::isAlive() const {
	return mIsAlive;
}

bool HUD::isActive() const {
	return mIsActive;
}

void HUD::setActive(const bool& active) {
	if (mScoreCounter[0] != nullptr)
		mScoreCounter[0]->setActive(active);
	if (mScoreCounter[1] != nullptr)
		mScoreCounter[1]->setActive(active);
	if (mLuddGauge != nullptr)
		mLuddGauge->setActive(active);
}

void HUD::onClick(std::string buttonFunc) {
	if (buttonFunc == "Mute") {
		mMute = !mMute;
		SoundEngine::getInstance().mute(mMute);
	}
}
