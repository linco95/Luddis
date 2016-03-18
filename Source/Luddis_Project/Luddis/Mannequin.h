#ifndef _INCLUDED_MANNEQUIN_
#define _INCLUDED_MANNEQUIN_

#include "InterfaceElement.h"
#include "Animation.h"
#include <SFML/Graphics/Sprite.hpp>

class Mannequin : public InterfaceElement{
public:
	enum ColorScheme {
		DEFAULT,
		PINK,
		BLUE,
		GREEN
	};

	Mannequin();
	Mannequin(std::string filename);
	~Mannequin();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void tick(const sf::Time& deltaTime) override;
	Strata getRenderLayer() const override;
	void setRenderLayer(Strata strata);
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool & active) override;

	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);
	void setScale(float x, float y);
	void setScale(const sf::Vector2f& scale);
	void setAnimate(bool animate);
	void setHeadAccessory(const char* filename);
	std::string getHeadAccessory() const;
	void setTailAccessory(const char* filename);
	std::string getTailAccessory() const;
	void setColorScheme(ColorScheme colorScheme);
	ColorScheme getColorScheme() const;
	const sf::Sprite* getSprite() const;
	void reset();

private:
	static const int COLORVARIATIONS = 4;

	Animation mAnimation[COLORVARIATIONS];
	sf::Sprite mHeadAccessory;
	std::string mCurrentHeadImage;
	sf::Sprite mTailAccessory;
	std::string mCurrentTailImage;
	ColorScheme mColor;
	Strata mStrata;

	bool mShowHeadAccessory;
	bool mShowTailAccessory;
	bool mAnimate;
	bool mIsAlive;
	bool mIsActive;
};


#endif // !_INCLUDED_MANNEQUIN_
