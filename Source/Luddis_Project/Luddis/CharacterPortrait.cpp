#define _USE_MATH_DEFINES
#include "CharacterPortrait.h"
#include "ResourceManager.h"
#include "Mannequin.h"

static const char* DEFAULT_FONT = "Resources/fonts/phitradesign Handwritten-Thin.ttf";
static const char* EMOTION_TEXTURE = "Resources/Images/Portraits/Emotions.png";
static const char* EMOTION_BUBBLE = "Resources/Images/Portraits/SpeechBubble.png";
static const int DEFAULT_FONTSIZE = 24;

static const int EMOTION_SHEET_ROWS = 1;
static const int EMOTION_SHEET_COLUMNS = 9;
//static sf::Vector2f DEFAULT_SCALE(1.0f, 1.0f);

static const float HIGHLIGHT_MAXTIME = 0.8f;
static const float FACTOR = 0.2f;

CharacterPortrait::CharacterPortrait(std::string textureFilename, sf::Vector2f pos, bool mirror) :
	mCharacter(ResourceManager::getInstance().getTexture(textureFilename)),
	mIsAlive(true),
	mIsActive(false),
	mEmotionFrame(0),
	mIsHighlighted(false),
	//mHighlightScale(DEFAULT_SCALE),
	mHighlightMoveTimer(0.0f),
	mEmotion(ResourceManager::getInstance().getTexture(EMOTION_TEXTURE)),
	mBubble(ResourceManager::getInstance().getTexture(EMOTION_BUBBLE)) {

	setPosition(pos);
	float spriteOrigoX = (float)mCharacter.getTextureRect().width / 2;
	float spriteOrigoY = (float)mCharacter.getTextureRect().height / 2;
	mCharacter.setOrigin(spriteOrigoX, spriteOrigoY);

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
	mBubble.move(spriteOrigoX+200, -130); //Align these according to new size
	mEmotion.move(spriteOrigoX, -350);
	if (mirror) {
		setScale(-1, 1);
	}
		move(0.0f, -spriteOrigoY);
	mDefaultSpritePos = mCharacter.getPosition();
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
		mCharacter.setPosition(mDefaultSpritePos + toMove);
		//mSprite.setScale(mHighlightScale);
	}
	else if (!mIsHighlighted && mHighlightMoveTimer > 0.0f) {
		mHighlightMoveTimer = std::max(mHighlightMoveTimer -= deltaTime.asSeconds(), 0.0f);
		float scalePercent = mHighlightMoveTimer / HIGHLIGHT_MAXTIME;
		if (mCharacter.getPosition() != mDefaultSpritePos)
			mCharacter.setPosition(mDefaultSpritePos);

		//mHighlightScale = DEFAULT_SCALE + DEFAULT_SCALE * scalePercent * FACTOR;
		//mSprite.setScale(mHighlightScale);
	}
}

void CharacterPortrait::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mCharacter, states);
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
