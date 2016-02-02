#include "Animation.h"
#include "ResourceManager.h"
//#include <SFML\System\Vector2.hpp>
#include <cmath>

using namespace sf;

struct AnimationImp : public Drawable {
	// The idea is to only send a parameter string with the file name, and the animation class will load the spritesheet file, and its meta data, from the animation folder
	AnimationImp(const std::string& aFilePath, const Vector2i& aTileSize, const int& aColumns, const int& aSpriteAmt, const Time& aFrameTime) :
		mTileSize(aTileSize),
		mColumns(aColumns),
		mFrameTime(aFrameTime),
		mSpriteAmt(aSpriteAmt),
		mCurrSprite(0) 
	{ 
		mSpriteSheet = ResourceManager::getInstance().getTexture(aFilePath);
		mSprite.setTexture(mSpriteSheet);
		mSprite.setTextureRect(IntRect(0, 0, mTileSize.x, mTileSize.y));
		mSprite.setOrigin(Vector2f(mTileSize.x / 2.0f, mTileSize.y / 2.0f));
	}

	void draw(RenderTarget& target, RenderStates states) const {

		target.draw(mSprite, states);
	}

	void tick(const Time& aTimeElapsed) {
		mCurrentTime += aTimeElapsed;
		
		if (mCurrentTime >= mFrameTime){
			stepAnimation();
			mCurrentTime = Time::Zero;
		}
	}

	void stepAnimation(){
		setFrame(++mCurrSprite);
	}

	const Sprite& getSprite() const {
		return mSprite;
	}
	void stepAnimation(const int& aStep){
		setFrame(mCurrSprite + aStep);
	}
	int getCurrentFrame() const{
		return mCurrSprite;
	}
	void setFrame(const int& aFrame){
		mCurrSprite = aFrame >= mSpriteAmt - 1 ? 0 : aFrame;
		mSprite.setTextureRect(IntRect(mCurrSprite * mTileSize.x, mCurrSprite / (mSpriteAmt - 1) * 0, mTileSize.x, mTileSize.y));

	}
	// Variables
	Texture mSpriteSheet;
	Vector2i mTileSize;
	Sprite mSprite;
	int mColumns,
		mSpriteAmt,
		mCurrSprite;
	Time mFrameTime,
		 mCurrentTime;
};

Animation::Animation(const std::string& aFilePath, const Vector2i& aTileSize, const int& aColumns, const int& aSpriteAmt, const Time& aFrameTime) :
	mAImp(new AnimationImp(aFilePath, aTileSize, aColumns, aSpriteAmt, aFrameTime)) {
	
}
Animation::~Animation(){
	delete mAImp;
}

void Animation::tick(const Time& aElapsedTime){
	mAImp->tick(aElapsedTime);
}
void Animation::draw(RenderTarget& target, RenderStates states) const {
	target.draw(*mAImp, states);
}

const Sprite& Animation::getSprite() const{
	return mAImp->getSprite();
}

Animation::Animation(const Animation& aAnim){
	mAImp = new AnimationImp(*aAnim.mAImp);
}
void Animation::stepAnimation(const int& aStep){
	mAImp->stepAnimation(aStep);
}
int Animation::getCurrentFrame() const{
	return mAImp->getCurrentFrame();
}
void Animation::setFrame(const int& aFrame){
	mAImp->setFrame(aFrame);
}

Animation& Animation::operator=(const Animation& aAnim){
	delete mAImp;
	mAImp = new AnimationImp(*aAnim.mAImp);
	return *this;
}