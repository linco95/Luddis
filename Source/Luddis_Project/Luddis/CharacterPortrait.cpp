#include "CharacterPortrait.h"
#include "ResourceManager.h"

static const char* DEFAULT_FONT = "Resources/fonts/arial.ttf";
static const char* EMOTION_TEXTURE = "Resources/Images/Portraits/EmotionsClarify.png";
static const char* EMOTION_BUBBLE = "Resources/Images/Portraits/SpeechBubble.png";
static const int DEFAULT_FONTSIZE = 24;

static const int EMOTION_SHEET_ROWS = 3;
static const int EMOTION_SHEET_COLUMNS = 3;

CharacterPortrait::CharacterPortrait(std::string textureFilename, std::string characterName, sf::Vector2f pos, bool mirror) :
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mIsAlive(true),
mIsActive(false),
mEmotionFrame(0),
mEmotion(ResourceManager::getInstance().getTexture(EMOTION_TEXTURE)),
mBubble(ResourceManager::getInstance().getTexture(EMOTION_BUBBLE)),
mName(characterName, ResourceManager::getInstance().getFont(DEFAULT_FONT), DEFAULT_FONTSIZE){
	setPosition(pos);
	float spriteOrigoX = (float)mSprite.getTextureRect().width / 2;
	float spriteOrigoY = (float)mSprite.getTextureRect().height / 2;
	mSprite.setOrigin(spriteOrigoX, spriteOrigoY);
	float textOrigoX = mName.getGlobalBounds().width / 2;
	float textOrigoY = mName.getGlobalBounds().height / 2;
	mName.setColor(sf::Color::White);
	mName.setOrigin(textOrigoX, textOrigoY);
	mName.move(0, spriteOrigoY*1.5f);
	move(spriteOrigoX*2.5f, -spriteOrigoY);

	int TEXTURE_WIDTH = (int)mEmotion.getTexture()->getSize().x;
	int TEXTURE_HEIGHT = (int)mEmotion.getTexture()->getSize().y;

	int index = 0;
	for (int i = 0; i < EMOTION_SHEET_ROWS; i++) {
		for (int j = 0; j < EMOTION_SHEET_COLUMNS; j++) {
			mFrame[index].height = TEXTURE_HEIGHT / 3;
			mFrame[index].width = TEXTURE_WIDTH / 3;
			mFrame[index].left = TEXTURE_WIDTH / EMOTION_SHEET_ROWS*j;
			mFrame[index].top = TEXTURE_HEIGHT / EMOTION_SHEET_COLUMNS*i;

			index++;
		}
	}
	mBubble.setOrigin((float)mBubble.getTextureRect().width, (float)mBubble.getTextureRect().height);
	mEmotion.setOrigin((float)mFrame[0].width/2, (float)mFrame[0].height / 2);
	mBubble.move(90, 90);
	mEmotion.move(150, 150);
	if (mirror) {
		setScale(-1, 1);
		mName.setScale(-1, -1);
	}
	mEmotion.setTextureRect(mFrame[mEmotionFrame]);
}

CharacterPortrait::~CharacterPortrait(){

}

void CharacterPortrait::tick(const sf::Time& deltaTime){

}

void CharacterPortrait::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mName, states);
	if (mEmotionFrame != 0) {
		target.draw(mBubble, states);
		target.draw(mEmotion, states);
	}
}

CharacterPortrait::RenderLayer CharacterPortrait::getRenderLayer() const{
	return FOREGROUND;
}

bool CharacterPortrait::isActive() const{
	return mIsActive;
}

bool CharacterPortrait::isAlive() const{
	return mIsAlive;
}

void CharacterPortrait::setActive(const bool& active){
	mIsActive = active;
}

void CharacterPortrait::kill(){
	mIsAlive = false;
}

void CharacterPortrait::expressEmotion(int frame){
	if (frame > MAX_FRAMES || frame < 0) {
		//Outside index range!
		return;
	}
	mEmotionFrame = frame;
	mEmotion.setTextureRect(mFrame[frame]);
}
