#ifndef _INCLUDED_OVERLAY_
#define _INCLUDED_OVERLAY_

#include "InterfaceElement.h"
#include <SFML/Graphics/Sprite.hpp>

class Overlay : public InterfaceElement{
public:
	Overlay(const char* filename, Strata strata);
	~Overlay();

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual void tick(const sf::Time & deltaTime) override;
	virtual Strata getRenderLayer() const override;
	virtual bool isAlive() const override;
	virtual bool isActive() const override;
	virtual void setActive(const bool & active) override;

	void kill();
private:
	sf::Sprite mSprite;
	Strata mStrata;

	bool mIsAlive;
	bool mIsActive;
};

#endif // !_INCLUDED_OVERLAY_
