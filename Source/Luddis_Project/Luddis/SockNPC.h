#ifndef _INCLUDED_SOCKNPC_
#define _INCLUDED_SOCKNPC_

#include "InterfaceElement.h"
#include "AnimationQueue.h"

class SockNPC : public InterfaceElement {
public:
	SockNPC();
	~SockNPC();

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	void tick(const sf::Time & deltaTime) override;
	Strata getRenderLayer() const override;
	bool isAlive() const override;
	bool isActive() const override;
	void setActive(const bool & active) override;

	void dive();

private:
	AnimationQueue mAnimation;
	bool mIsActive;
	bool mIsAlive;

	float tempTimer;
	float mDiveTimer;
	bool mDive;
	bool mDiveDown;
	bool mLeftSide;
};

#endif // !_INCLUDED_SOCKNPC_
