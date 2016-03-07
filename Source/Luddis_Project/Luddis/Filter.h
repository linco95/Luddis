#ifndef _INCLUDED_FILTER_
#define _INCLUDED_FILTER_

#include "InterfaceElement.h"
#include <SFML/Graphics/RectangleShape.hpp>

class Filter :public InterfaceElement{
public:

	Filter(float fadeTimer);
	~Filter();

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual void tick(const sf::Time & deltaTime) override;
	virtual Strata getRenderLayer() const override;
	virtual bool isAlive() const override;
	virtual bool isActive() const override;
	virtual void setActive(const bool & active) override;
	void kill();

	bool getHalfway();
	bool getComplete();

private:
	sf::RectangleShape mFadeEffect;
	bool mIsAlive;
	bool mFade;
	bool mFadeOut;
	bool mHalfway;
	bool mComplete;
	float mTimer;
	float mMaxTimer;

};

#endif // !_INCLUDED_FILTER_
