#ifndef _INCLUDED_FILTER_
#define _INCLUDED_FILTER_

#include "InterfaceElement.h"
#include <SFML/Graphics/RectangleShape.hpp>

class Filter :public InterfaceElement{
public:
	enum FilterType {
		FADEIN,
		FADEOUT,
		FADEBOTH
	};

	//Leave filename blank for a textureless filter.
	Filter(float fadeTimer, FilterType filterType, std::string filename = "");
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
	sf::Uint8 mRed, mGreen, mBlue;
	bool mIsAlive;
	bool mFade;
	bool mFadeOut;
	bool mHalfway;
	bool mComplete;
	float mTimer;
	float mMaxTimer;
	FilterType mFilterType;
};

#endif // !_INCLUDED_FILTER_
