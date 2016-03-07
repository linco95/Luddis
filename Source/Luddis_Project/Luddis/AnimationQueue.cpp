#include "AnimationQueue.h"
#include "ResourceManager.h"

AnimationQueue::AnimationQueue(const Animation& aDefaultAnimation):
	mDefaultAnim(aDefaultAnimation),
	mReplacingAnim(aDefaultAnimation),
	mIsReplaced(false),
	mOrigin(0,0) {

}
AnimationQueue::~AnimationQueue() {

}

void AnimationQueue::tick(const sf::Time& deltaTime) {
	// Tick the current animation
	getCurrAnimation().tick(deltaTime);
	// Check if the animation is replaced. If it has looped, we go back to the default animation
	if (mIsReplaced && mReplacingAnim.hasLooped()) {
		mIsReplaced = false;
		mDefaultAnim.setFrame(0);
		tick(deltaTime);
	}
}

Animation& AnimationQueue::getCurrAnimation(){
	return mIsReplaced ? mReplacingAnim : mDefaultAnim;
}
void AnimationQueue::setDefaultAnimation(const Animation& aAnim){
	mDefaultAnim = aAnim;
	mDefaultAnim.setOrigin(mOrigin);
}
const Animation& AnimationQueue::getCurrAnimation() const{
	return mIsReplaced ? mReplacingAnim : mDefaultAnim;
}
void AnimationQueue::replaceAnimation(const Animation& aAnim){
	if (mIsReplaced) return;
	overrideAnimation(aAnim);
}
void AnimationQueue::overrideAnimation(const Animation & aAnim) {
	mReplacingAnim = aAnim;
	mReplacingAnim.setOrigin(mOrigin);
	mIsReplaced = true;
}
void AnimationQueue::setOrigin(const sf::Vector2f& a_Origin) {
	mOrigin = a_Origin;
	mDefaultAnim.setOrigin(mOrigin);
	mReplacingAnim.setOrigin(mOrigin);
}