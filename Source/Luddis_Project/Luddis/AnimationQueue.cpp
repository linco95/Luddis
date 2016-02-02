#include "AnimationQueue.h"

AnimationQueue::AnimationQueue(const Animation& aDefaultAnimation):
	mDefaultAnim(aDefaultAnimation) {

}
AnimationQueue::~AnimationQueue() {

}

void AnimationQueue::tick(const sf::Time& deltaTime){
	if (mAnimations.empty()){
		mDefaultAnim.tick(deltaTime);
	}
	else{
		mAnimations.front().tick(deltaTime);
		if (mAnimations.front().hasLooped()){
			mAnimations.pop();
			tick(deltaTime);
		}
	}
}

Animation& AnimationQueue::getCurrAnimation(){
	return mAnimations.empty() ? mDefaultAnim : mAnimations.front();
}
void AnimationQueue::addAnimation(const Animation& aAnim){
	mAnimations.push(aAnim);
}
