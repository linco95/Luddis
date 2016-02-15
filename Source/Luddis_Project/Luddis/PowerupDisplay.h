#ifndef _INCLUDED_POWERUPDISPLAY_
#define _INCLUDED_POWERUPDISPLAY_

#include "InterfaceElement.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>
#include <SFML/Graphics/Sprite.hpp>

class PowerupDisplay: public InterfaceElement{
public:
	PowerupDisplay(std::string textureFilename, sf::Vector2f pos, float cooldown);
	virtual ~PowerupDisplay();

	void tick(const sf::Time& deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isAlive() const override;
	bool isActive() const  override;
	void setActive(const bool& active) override;
	RenderLayer getRenderLayer() const override;

	void activateCooldown();
	float getCooldown() const;
private:
	void updateCooldown(const sf::Time& deltaTime);

	sf::Sprite mSprite;
	bool mIsAlive;
	bool mIsActive;
	sf::RectangleShape mRectShape;
	float mCooldown;
	const float MAX_COOLDOWN;
};

#endif // !_INCLUDED_POWERUPDISPLAY_
