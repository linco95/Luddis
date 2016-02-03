#ifndef INCLUDED_ANIMATIONQUEUE
#define INCLUDED_ANIMATIONQUEUE

#include <queue>
#include "Animation.h"

namespace sf{
	class Time;
}

class AnimationQueue {
public:
	AnimationQueue(const Animation& aDefaultAnimation);
	~AnimationQueue() ;

	void tick(const sf::Time& deltaTime);

	Animation& getCurrAnimation();
	const Animation& getCurrAnimation() const;
	void addAnimation(const Animation& aAnim);
	void setDefaultAnimation(const Animation& aAnim);
	AnimationQueue(const AnimationQueue&) = delete;
	AnimationQueue& operator=(const AnimationQueue&) = delete;

private:
	Animation mDefaultAnim;
	std::queue<Animation> mAnimations;
};


#endif // INCLUDED_ANIMATIONQUEUE