#include "Overlay.h"
#include "ResourceManager.h"

Overlay::Overlay(const char * filename):
mSprite(ResourceManager::getInstance().getTexture(filename)){

}

Overlay::~Overlay(){

}

void Overlay::draw(sf::RenderTarget & target, sf::RenderStates states) const{

}

void Overlay::tick(const sf::Time & deltaTime){

}

Overlay::Strata Overlay::getRenderLayer() const{
	return THIRD;
}

bool Overlay::isAlive() const{
	return mIsAlive;
}

bool Overlay::isActive() const{
	return mIsActive;
}

void Overlay::setActive(const bool & active){
	mIsActive = active;
}

void Overlay::kill(){
	mIsAlive = false;
}
