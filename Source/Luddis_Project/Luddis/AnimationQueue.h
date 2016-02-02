#ifndef INCLUDED_ANIMATIONQUEUE
#define INCLUDED_ANIMATIONQUEUE

#include <queue>
#include "Animation.h"

class AnimationQueue : public Animation {
public:
	AnimationQueue(const Animation& aDefaultAnimation);
	~AnimationQueue() override;

	void tick(const sf::Time& aTimeElapsed) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	const sf::Sprite& getSprite() const override;


private:
	Animation mDefaultAnim;
	std::queue<Animation> mAnimations;

};


#endif // INCLUDED_ANIMATIONQUEUE