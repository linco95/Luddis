#include "AnimationQueue.h"
#include "ResourceManager.h"

AnimationQueue::AnimationQueue(const Animation& aDefaultAnimation):
	mDefaultAnim(aDefaultAnimation),
	mReplacingAnim(aDefaultAnimation),
	mIsReplaced(false) {

}
AnimationQueue::~AnimationQueue() {

}

void AnimationQueue::tick(const sf::Time& deltaTime){
	if (!mIsReplaced){
		mDefaultAnim.tick(deltaTime);
	}
	else{
		mReplacingAnim.tick(deltaTime);
		if (mReplacingAnim.hasLooped()){
			mIsReplaced = false;
			mDefaultAnim.setFrame(0);
			tick(deltaTime);
		}
	}
}

Animation& AnimationQueue::getCurrAnimation(){
	return mIsReplaced ? mReplacingAnim : mDefaultAnim;
}
void AnimationQueue::setDefaultAnimation(const Animation& aAnim){
	mDefaultAnim = aAnim;
}
const Animation& AnimationQueue::getCurrAnimation() const{
	return mIsReplaced ? mReplacingAnim : mDefaultAnim;
}
void AnimationQueue::replaceAnimation(const Animation& aAnim){
	if (!mIsReplaced){
		mReplacingAnim = aAnim;
		mIsReplaced = true;
	}
}
