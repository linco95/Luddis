#include "Animation.h"
#include "ResourceManager.h"
#include <cmath>
#include <rapidjson\document.h>
#include <cassert>

using namespace sf;

struct AnimationImp : public Drawable, public sf::Transformable {
	
	AnimationImp(const std::string& aFilePathNoExtension) :
		mCurrSprite(0),
		mCurrTile(0,0),
		mHasLooped(false)
	{
		std::string config = ResourceManager::getInstance().loadJsonFile(aFilePathNoExtension + ".json");
		rapidjson::Document configDoc;
		configDoc.Parse(config.c_str());
		assert(configDoc.IsObject());

		assert(configDoc.HasMember("Tile_width") && configDoc["Tile_width"].IsInt());
		assert(configDoc.HasMember("Tile_height") && configDoc["Tile_height"].IsInt());
		assert(configDoc.HasMember("Number_of_tiles") && configDoc["Number_of_tiles"].IsInt());
		assert(configDoc.HasMember("Number_of_columns") && configDoc["Number_of_columns"].IsInt());
		assert(configDoc.HasMember("Frame_time_seconds") && configDoc["Frame_time_seconds"].IsDouble());
		mTileSize = sf::Vector2i(configDoc["Tile_width"].GetInt(), configDoc["Tile_height"].GetInt());

		mSpriteAmt = configDoc["Number_of_tiles"].GetInt();
		mColumns = configDoc["Number_of_columns"].GetInt();
		mFrameTime = sf::seconds((float)configDoc["Frame_time_seconds"].GetDouble());

		mSpriteSheet = &ResourceManager::getInstance().getTexture(aFilePathNoExtension + ".png");
		mSprite.setTexture(*mSpriteSheet);
		mSprite.setTextureRect(IntRect(0, 0, mTileSize.x, mTileSize.y));
		mSprite.setOrigin(Vector2f(mTileSize.x / 2.0f, mTileSize.y / 2.0f));
	}

	
	// The idea is to only send a parameter string with the file name, and the animation class will load the spritesheet file, and its meta data, from the animation folder
	AnimationImp(const std::string& aFilePath, const Vector2i& aTileSize, const int& aColumns, const int& aSpriteAmt, const Time& aFrameTime) :
		mTileSize(aTileSize),
		mColumns(aColumns),
		mFrameTime(aFrameTime),
		mSpriteAmt(aSpriteAmt),
		mCurrSprite(0),
		mCurrTile(0, 0),
		mHasLooped(false)
	{ 
		mSpriteSheet = &ResourceManager::getInstance().getTexture(aFilePath);
		mSprite.setTexture(*mSpriteSheet);
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
	// Needs to be implemented in the actuan animation class, and where it should be used
	Sprite& getSprite() {
		return mSprite;
	}

	void stepAnimation(const int& aStep){
		setFrame(mCurrSprite + aStep);
	}
	int getCurrentFrame() const{
		return mCurrSprite;
	}

	void setFrame(const int& aFrame){
		// Increment the current sprite index
		if (aFrame > mSpriteAmt - 1){
			mCurrSprite = 0;
			mHasLooped = true;
		}
		else{
			mCurrSprite = aFrame;
		}

		// Set the current tile in the spritesheet
		mCurrTile.y = mCurrSprite / mColumns;
		mCurrTile.x = mCurrSprite % mColumns;

		mSprite.setTextureRect(IntRect(mCurrTile.x * mTileSize.x, mCurrTile.y * mTileSize.y, mTileSize.x, mTileSize.y));
	}

	bool hasLooped(){
		return mHasLooped;
	}

	AnimationImp(const AnimationImp& aAnim) : 
		mSpriteSheet(aAnim.mSpriteSheet),
		mTileSize(aAnim.mTileSize),
		mSprite(aAnim.mSprite),
		mColumns(aAnim.mColumns),
		mSpriteAmt(aAnim.mSpriteAmt),
		mCurrSprite(aAnim.mCurrSprite),
		mFrameTime(aAnim.mFrameTime),
		mCurrentTime(aAnim.mCurrentTime),
		mHasLooped(aAnim.mHasLooped)
	{
		mSprite.setTexture(*mSpriteSheet);
	}

	// Variables
	Texture* mSpriteSheet;
	Vector2i mTileSize,
			 mCurrTile;
	Sprite mSprite;
	int mColumns,
		mSpriteAmt,
		mCurrSprite;
	Time mFrameTime,
		 mCurrentTime;
	bool mHasLooped;
};

Animation::Animation(const std::string& aFilePath, const Vector2i& aTileSize, const int& aColumns, const int& aSpriteAmt, const Time& aFrameTime) :
	mAImp(new AnimationImp(aFilePath, aTileSize, aColumns, aSpriteAmt, aFrameTime)) {	
}
Animation::Animation(const std::string& aFilePath) : 
	mAImp(new AnimationImp(aFilePath)) {
}
Animation::~Animation(){
	delete mAImp;
}

void Animation::tick(const Time& aElapsedTime){
	mAImp->tick(aElapsedTime);
}
void Animation::draw(RenderTarget& target, RenderStates states) const {
	states.transform *= getTransform();
	target.draw(*mAImp, states);
}

const Sprite& Animation::getSprite() const{
	return mAImp->getSprite();
}

Animation::Animation(const Animation& aAnim) :
	mAImp(new AnimationImp(*aAnim.mAImp))
{
	
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
bool Animation::hasLooped() const{
	return mAImp->hasLooped();
}
Animation& Animation::operator=(const Animation& aAnim){
	delete mAImp;
	mAImp = new AnimationImp(*aAnim.mAImp);
	return *this;
}

int Animation::getSpriteAmount()
{
	return mAImp->mSpriteAmt;
}
