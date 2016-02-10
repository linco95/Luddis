#ifndef INCLUDED_ANIMATION
#define INCLUDED_ANIMATION

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\System\Vector2.hpp>
#include <string>

struct AnimationImp;

namespace sf{
	class Time;
	class Sprite;
}

class Animation : public sf::Drawable, public sf::Transformable {
public:
	Animation(const std::string& aFilePathNoExtension);
	Animation(const std::string& aFilePath, const sf::Vector2i& aTileSize, const int& aColumns, const int& aSpriteAmt, const sf::Time& aFrameTime);
	~Animation();

	void tick(const sf::Time& aTimeElapsed);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	const sf::Sprite& getSprite() const; 
	// Step the animation relative to its current frame
	void stepAnimation(const int& aStep);
	int getCurrentFrame() const;
	void setFrame(const int& aFrame);
	bool hasLooped() const;
	Animation(const Animation& aAnim);
	Animation& operator=(const Animation& aAnim);
	void changeScale(float x, float y);
private:
	AnimationImp* mAImp;
};

#endif