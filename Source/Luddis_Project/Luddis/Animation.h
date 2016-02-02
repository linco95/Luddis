#ifndef INCLUDED_ANIMATION
#define INCLUDED_ANIMATION

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\System\Vector2.hpp>
#include <string>

struct AnimationImp;

namespace sf{
	class Time;
	class Sprite;
}

class Animation : public sf::Drawable {
public:
	Animation(const std::string& aFilePath, const sf::Vector2i& aTileSize, const int& aColumns, const int& aSpriteAmt, const sf::Time& aFrameTime);
	virtual ~Animation();

	virtual void tick(const sf::Time& aTimeElapsed);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual const sf::Sprite& getSprite() const;
	virtual void stepAnimation(const int& aStep);
	virtual int getCurrentFrame() const;
	virtual void setFrame(const int& aFrame);
	Animation(const Animation& aAnim);
	virtual Animation& operator=(const Animation& aAnim);
private:
	AnimationImp* mAImp;
};

#endif