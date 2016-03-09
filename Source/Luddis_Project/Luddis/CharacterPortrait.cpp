#define _USE_MATH_DEFINES
#include "CharacterPortrait.h"
#include "ResourceManager.h"

static const char* DEFAULT_FONT = "Resources/fonts/arial.ttf";
static const char* EMOTION_TEXTURE = "Resources/Images/Portraits/Emotions.png";
static const char* EMOTION_BUBBLE = "Resources/Images/Portraits/SpeechBubble.png";
static const int DEFAULT_FONTSIZE = 24;

static const int EMOTION_SHEET_ROWS = 1;
static const int EMOTION_SHEET_COLUMNS = 9;
//static sf::Vector2f DEFAULT_SCALE(1.0f, 1.0f);

static const float HIGHLIGHT_MAXTIME = 0.8f;
static const float FACTOR = 0.2f;

CharacterPortrait::CharacterPortrait(std::string textureFilename, std::string characterName, sf::Vector2f pos, bool mirror) :
	mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
	mIsAlive(true),
	mIsActive(false),
	mEmotionFrame(0),
	mIsHighlighted(false),
	//mHighlightScale(DEFAULT_SCALE),
	mHighlightMoveTimer(0.0f),
	mEmotion(ResourceManager::getInstance().getTexture(EMOTION_TEXTURE)),
	mBubble(ResourceManager::getInstance().getTexture(EMOTION_BUBBLE)),
	mName(characterName, ResourceManager::getInstance().getFont(DEFAULT_FONT), DEFAULT_FONTSIZE) {

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
	int FRAME_WIDTH = TEXTURE_WIDTH / EMOTION_SHEET_COLUMNS;
	int FRAME_HEIGHT = TEXTURE_HEIGHT / EMOTION_SHEET_ROWS;

	int index = 0;
	for (int i = 0; i < EMOTION_SHEET_ROWS; i++) {
		for (int j = 0; j < EMOTION_SHEET_COLUMNS; j++) {
			mFrame[index].top = (index / EMOTION_SHEET_COLUMNS)*FRAME_HEIGHT;
			mFrame[index].left = (index % EMOTION_SHEET_COLUMNS)*FRAME_WIDTH - FRAME_WIDTH;
			mFrame[index].height = FRAME_HEIGHT;
			mFrame[index].width = FRAME_WIDTH;

			index++;
		}
	}
	mBubble.setOrigin((float)mBubble.getTextureRect().width, (float)mBubble.getTextureRect().height);
	mEmotion.setOrigin((float)mFrame[0].width / 2, (float)mFrame[0].height / 2);
	mBubble.setScale(0.8f, 0.8f);
	mEmotion.setScale(0.8f, 0.8f);
	mBubble.move(485, -30);
	mEmotion.move(285, -250);
	if (mirror) {
		setScale(-1, 1);
		mName.setScale(-1, 1);
	}
	mDefaultSpritePos = mSprite.getPosition();
	mEmotion.setTextureRect(mFrame[mEmotionFrame]);
}

CharacterPortrait::~CharacterPortrait() {

}

void CharacterPortrait::tick(const sf::Time& deltaTime) {
	if (mIsHighlighted && mHighlightMoveTimer < HIGHLIGHT_MAXTIME) {
		mHighlightMoveTimer = std::min(mHighlightMoveTimer += deltaTime.asSeconds(), HIGHLIGHT_MAXTIME);
		float scalePercent = mHighlightMoveTimer / HIGHLIGHT_MAXTIME;
		//The portrait will "bob" __ times here with a magnitude of __ pixels.
		sf::Vector2f toMove(0.0f, sinf(scalePercent * 8 * (float)M_PI) * 15); //sin(x) lambda = 2PI
		//mHighlightScale = DEFAULT_SCALE + DEFAULT_SCALE * scalePercent * FACTOR;
		mSprite.setPosition(mDefaultSpritePos + toMove);
		//mSprite.setScale(mHighlightScale);
	}
	else if (!mIsHighlighted && mHighlightMoveTimer > 0.0f) {
		mHighlightMoveTimer = std::max(mHighlightMoveTimer -= deltaTime.asSeconds(), 0.0f);
		float scalePercent = mHighlightMoveTimer / HIGHLIGHT_MAXTIME;
		if (mSprite.getPosition() != mDefaultSpritePos)
			mSprite.setPosition(mDefaultSpritePos);

		//mHighlightScale = DEFAULT_SCALE + DEFAULT_SCALE * scalePercent * FACTOR;
		//mSprite.setScale(mHighlightScale);
	}
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

CharacterPortrait::Strata CharacterPortrait::getRenderLayer() const {
	return FOURTH;
}

bool CharacterPortrait::isActive() const {
	return mIsActive;
}

bool CharacterPortrait::isAlive() const {
	return mIsAlive;
}

void CharacterPortrait::setActive(const bool& active) {
	mIsActive = active;
}

void CharacterPortrait::kill() {
	mIsAlive = false;
}

void CharacterPortrait::expressEmotion(int frame) {
	if (frame > MAX_FRAMES || frame < 0) {
		//Outside index range!
		return;
	}
	mEmotionFrame = frame;
	mEmotion.setTextureRect(mFrame[frame]);
}

void CharacterPortrait::highlight(bool highlight) {
	mIsHighlighted = highlight;
}
