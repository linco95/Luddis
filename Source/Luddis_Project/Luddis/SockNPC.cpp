#include "SockNPC.h"
#include "Button.h"

static const std::string SOCKANIMATION_FILEPATH = "Resources/Images/Spritesheets/Sock_idle";


SockNPC::SockNPC() :
	mIsActive(true),
	mIsAlive(true),
	mAnimation(SOCKANIMATION_FILEPATH) {

}

SockNPC::~SockNPC(){

}

void SockNPC::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

void SockNPC::tick(const sf::Time & deltaTime){
	mAnimation.tick(deltaTime);
}

SockNPC::Strata SockNPC::getRenderLayer() const{
	return Strata::FOURTH;
}

bool SockNPC::isAlive() const{
	return mIsAlive;
}

bool SockNPC::isActive() const{
	return mIsActive;
}

void SockNPC::setActive(const bool & active){

}
