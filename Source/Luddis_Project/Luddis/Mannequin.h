#ifndef _INCLUDED_MANNEQUIN_
#define _INCLUDED_MANNEQUIN_

#include "InterfaceElement.h"
#include "Animation.h"
#include <SFML/Graphics/Sprite.hpp>

class Mannequin : public InterfaceElement{
public:
	Mannequin();
	~Mannequin();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void tick(const sf::Time& deltaTime) override;
	virtual Strata getRenderLayer() const override;
	virtual bool isAlive() const override;
	virtual bool isActive() const override;
	virtual void setActive(const bool & active) override;

	void setAnimate(bool animate);
	void setHeadAccessory(const char* filename);
	void setTailAccessory(const char* filename);

private:
	Animation mAnimation;
	sf::Sprite mHeadAccessory;
	sf::Sprite mTailAccessory;

	bool mAnimate;
	bool mIsAlive;
	bool mIsActive;
};


#endif // !_INCLUDED_MANNEQUIN_
