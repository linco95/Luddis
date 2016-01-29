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
	~Animation();

	void tick(const sf::Time& aTimeElapsed);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Sprite getSprite() const;


private:
	AnimationImp* mAImp;
};

#endif