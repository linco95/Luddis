#ifndef _INCLUDED_POWERUPDISPLAY_
#define _INCLUDED_POWERUPDISPLAY_

#include "Entity.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>

class PowerupDisplay: public Entity{
public:
	PowerupDisplay(std::string textureFilename, sf::Vector2f pos, float cooldown);
	~PowerupDisplay();

	virtual void tick(const sf::Time& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual bool isAlive();
	virtual RenderLayer getRenderLayer() const;

	void activateCooldown();
	float getCooldown() const;
private:
	void updateCooldown(const sf::Time& deltaTime);

	sf::Sprite mSprite;
	bool mIsAlive;
	sf::RectangleShape mRectShape;
	float mCooldown;
	const float MAX_COOLDOWN;
};

#endif // !_INCLUDED_POWERUPDISPLAY_
