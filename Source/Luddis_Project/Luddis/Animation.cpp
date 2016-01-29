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
		mCurrSprite == mSpriteAmt - 1 ? mCurrSprite = 0 : mCurrSprite++;
		mSprite.setTextureRect(IntRect(mCurrSprite * mTileSize.x, mCurrSprite / (mSpriteAmt - 1) * 0, mTileSize.x, mTileSize.y));
	}

	const Sprite& getSprite() const {
		return mSprite;
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