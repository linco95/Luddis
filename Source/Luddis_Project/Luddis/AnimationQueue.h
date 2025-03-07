#ifndef INCLUDED_ANIMATIONQUEUE
#define INCLUDED_ANIMATIONQUEUE

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
	void replaceAnimation(const Animation& aAnim);
	void overrideAnimation(const Animation& aAnim);
	void setDefaultAnimation(const Animation& aAnim);
	void setOrigin(const sf::Vector2f& a_Origin);
	AnimationQueue(const AnimationQueue&) = delete;
	AnimationQueue& operator=(const AnimationQueue&) = delete;

private:
	bool mIsReplaced;
	sf::Vector2f mOrigin;
	Animation mDefaultAnim;
	Animation mReplacingAnim;
};


#endif // INCLUDED_ANIMATIONQUEUE