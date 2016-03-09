#ifndef _INCLUDED_BACKGROUNDEFFECT_
#define _INCLUDED_BACKGROUNDEFFECT_

#include <SFML/Window.hpp>
#include "Entity.h"
#include <string>
#include <SFML/Graphics/Sprite.hpp>

class BackgroundEffect : public Entity {
public:
	//The max life time should be entered in seconds
	BackgroundEffect(std::string textureFilename, sf::Vector2f direction, sf::Vector2f position, float maxLifeTimeMS, Transformable* aTarget);
	virtual ~BackgroundEffect();

	void tick(const sf::Time& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool isAlive() const;
	bool isActive() const;
	void setActive(const bool& active);
	Renderer::RenderLayer getRenderLayer() const;
	void setTexture(std::string filename);
	void stun(const sf::Time& deltatime) override;
private:
	void updateMovement(const sf::Time& deltaTime);
	void checkLifeTime();
	sf::Transformable* mTarget;

	sf::Sprite mSprite;
	float mLifeTime;
	bool mIsAlive;
	bool mIsActive;
	sf::Vector2f mDirection;
	float mTimeStunned;
};

#endif 
